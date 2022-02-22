// Monoworld.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MONOWORLD_HXX__INCLUDED) // {
#define MONOWORLD_HXX__INCLUDED

#include "basic.hxx"
#include "PlayerObserver.hxx"
#include "Mutex.hxx"
#include "PlayerSenseBody.hxx"
#include "PlayerSee.hxx"
#include "PlayerHear.hxx"

/////////////////////////////////////////////////////////////////////////////
// Monoworld

/**
* （単一層）ワールドモデル。sense_body を自発的に送信しないことを前提としてい 
* る。なお、ワールドモデルは複数用意される可能性があるため、色の名前を形容詞と
* して使用することで区別する。
* @param	ColorBall
*				ボールクラス。
* @param	ColorPlayer
*				プレイヤークラス。
* @param	ColorSelef
*				セルフクラス。
*/
template <class ColorBall, class ColorPlayer, class ColorSelf>
class Monoworld : protected PlayerObserver
{
public:
	typedef ColorBall Ball;
	typedef ColorPlayer Player;
	typedef ColorSelf Self;
	typedef std::list<Player> Players;
	typedef std::vector<Vector> Vectors;
	
private:
	Monoworld(const Monoworld&);
	Monoworld& operator= (const Monoworld&);
	bool operator== (const Monoworld&) const;
	
protected:
	Mutex m_mutex;
	int m_refereeTimer;		// 審判の時計。ハーフタイム等で停止
	int m_time;				// 時刻。sense_body ごとに＋１
	bool m_timeUpdatedBySee;	// see と sense_body が同時に来る場合の see で先行して m_time++ してしまった？
	PlayMode m_playMode;
	int m_scoreLeft;		// 味方チームの得点
	int m_scoreRight;		// 敵チームの得点
	Ball m_ball;
	Players m_others;
	Self m_self;
	Vectors m_flags;
	enum { m_historySize = 40 };
	InformationType m_receptionOrders[4][m_historySize];
	int m_receptionOrderSizes[4];
	int m_indexHistoryNext;
	InformationType m_history[m_historySize];
	
	virtual bool estimatePosition(const PlayerSee& si, const Angle& direction, Vector& result);
	virtual bool estimateDirection(const PlayerSee& si, const Self& anticipatedSelf, Angle& result);
	virtual void intoWaiting() {
		unlock();
	}
	virtual void outOfWaiting() {
		lock();
	}
	virtual void initialize(PlayMode playMode, int uniformNumber, bool goalie) {
		lock();
		m_playMode = playMode;
		m_self.initialize(uniformNumber, goalie, -1);
	}
	virtual void connectionBroken() {
		unlock();
	}
	void tryToStep(InformationType it, int refereeTimer) {
		int i = m_indexHistoryNext;
		ASSERT(it == INFORMATION_SEE || it == INFORMATION_SENSE_BODY || it == INFORMATION_ERROR);
		m_history[m_indexHistoryNext++] = it;
		m_indexHistoryNext = m_indexHistoryNext % m_historySize;
		if(it == INFORMATION_ERROR)
			return;
		
		bool newCycle;
		if(it == INFORMATION_SENSE_BODY) {
			m_refereeTimer = refereeTimer;
			if(m_timeUpdatedBySee) {
				newCycle = false;
				m_timeUpdatedBySee = false;
			} else {
				newCycle = true;
			}
		} else {
			if(refereeTimer > m_refereeTimer) {
				m_refereeTimer = refereeTimer;
				newCycle = true;
			} else {
				newCycle = false;
				if(playModeHelper.isRefreeTimerProceeding(m_playMode)) {
					newCycle = false;
				} else {
					ASSERT(m_refereeTimer == refereeTimer);
					int k;
					switch(self().viewWidth()) {
					default:
						ASSERT(false);
					case VIEW_WIDTH_NARROW:
						k = 0;
						break;
					case VIEW_WIDTH_NORMAL:
						k = 1;
						break;
					case VIEW_WIDTH_WIDE:
						k = 2;
						break;
					}
					switch(self().viewQuality()) {
					default:
						ASSERT(false);
					case VIEW_QUALITY_LOW:
						break;
					case VIEW_QUALITY_HIGH:
						k++;
						break;
					}
					
					int j;
					for(j=0; j<m_receptionOrderSizes[k]; j++) {
						i = (i - 1 + m_historySize) % m_historySize;
						if(m_history[i] != m_receptionOrders[k][j])
							break;
					}
					newCycle = (j >= m_receptionOrderSizes[k]);
				}
			}
		}
		if(newCycle) {
			if(it == INFORMATION_SEE)
				m_timeUpdatedBySee = true;
			m_time++;
			step();
		} else {
		}
	}
	virtual void received(const PlayerSenseBody& sbi) {
		tryToStep(INFORMATION_SENSE_BODY, sbi.refereeTimer());
		m_self.update(sbi);
	}
	virtual void updateSelf(const PlayerSee& si) {
		// self の更新
		// 体の向きの更新
		Angle direction;
		if(estimateDirection(si, self(), direction)) {
			m_self.updateSightDirection(direction);
			ASSERT(abs(self().sightDirection() - direction).degrees() <= 0.001);
		}
		// 位置の更新
		Vector position;
		if(estimatePosition(si, self().sightDirection(), position)) {
			m_self.updatePosition(position);
		}
		// 速度の更新
#if 1
		// もっとも近いものから求める
		double minDistance = DBL_MAX;
		const PlayerSeeObject* minFlag = 0;
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->distanceChange().valid() && o->flagId().valid()) {
				if(o->distance() < minDistance) {
					minDistance = o->distance();
					minFlag = o;
				}
			}
		}
		if(minFlag != 0) {
			const PlayerSeeObject* flag = minFlag;
			Vector position = Vector(flag->distance(), flag->direction() + self().sightDirection());
			double distance = position.norm();
			if(distance != 0.0) {
				Vector e = position / distance;
				double x = flag->directionChange() * e.x() - flag->directionChange() * (PI/180) * distance * e.y();
				double y = flag->directionChange() * e.y() - flag->directionChange() * (PI/180) * distance * e.x();
				//double coefficient = 1/(distance+0.001);
				m_self.updateVelocity(Vector(-x, -y));
			}
		}
#else
		// 平均を取る
		double sum = 0.0;
		Vector vector(0, 0);
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* flag = *flags;
			if(flag->distanceChange().valid()) {
				Vector position = Vector(flag->distance(), flag->direction() + self().sightDirection());
				double distance = position.norm();
				if(distance != 0.0) {
					Vector e = position / distance;
					double x = flag->directionChange() * e.x() - flag->directionChange() * (PI/180) * distance * e.y();
					double y = flag->directionChange() * e.y() - flag->directionChange() * (PI/180) * distance * e.x();
					//double coefficient = 1/(distance+0.001);
					double coefficient = pow(0.9, distance);
					sum += coefficient;
					vector += Vector(-x, -y) * coefficient;
				}
			}
		}
		if(sum != 0.0) {
			m_self.updateVelocity(vector / sum);
		}
#endif
		// 時刻の更新
		m_self.updateTime(m_time);
	}
	virtual void updateBall(const PlayerSee& si) {
		// ball の更新
		if(si.ball())
			m_ball.update(self(), m_time, *si.ball());
	}
	virtual void updateFlags(const PlayerSee& si) {
		// flags の更新
		m_flags.clear();
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* flag = *flags;
			if(flag->distance().valid()) {
				Vector toFlag = Vector(flag->distance(), flag->direction() + self().sightDirection());
				m_flags.push_back(self().position() + toFlag);
			}
		}
	}
	virtual void received(const PlayerSee& si);
	virtual void received(const PlayerHear& hi, int /*key*/) {
		if(hi.senderType() == SENDER_REFEREE) {
			int kind = hi.refereeMessage();
			if(0 <= kind && kind < PLAYMODE_MAZ) {
				if(kind == PLAYMODE_GOAL_L)
					m_scoreLeft = hi.score();
				else if(kind == PLAYMODE_GOAL_R)
					m_scoreRight = hi.score();
				if(playModeHelper.isPlayModeChanged(m_playMode, (PlayMode)kind))
					m_playMode = (PlayMode)kind;
			}
		}
	}
	virtual void turned(double moment) {
		m_self.turned(moment);
	}
	virtual void dashed(double power) {
		m_self.dashed(power);
	}
	virtual void kicked(double power, Angle direction) {
		m_ball.kicked(self(), power, direction);
	}
	virtual void ballCaught(Angle direction) {
		m_self.ballCaught(m_time, direction);
	}
	virtual void moved(Vector position) {
		m_self.moved(position);
	}
	virtual void neckTurned(Angle angle) {
		m_self.neckTurned(angle);
	}
	virtual void viewChanged(ViewWidth width, ViewQuality quality) {
		if(self().viewWidth() != width || self().viewQuality() != quality)
			tryToStep(INFORMATION_ERROR, -1);
		m_self.viewChanged(width, quality);
	}
	
public:
	~Monoworld() {
	}
	Monoworld() {
		m_refereeTimer = -1;
		m_time = 0;
		m_timeUpdatedBySee = false;
		m_playMode = PLAYMODE_BEFORE_KICK_OFF;
		m_scoreLeft = 0;
		m_scoreRight = 0;
		m_ball.initialize();
		m_self.initialize(-1, false, -1);
		
		const int delta = server().TIMEDELTA();
		int i;
		for(i=0; i<COUNT_OF(m_receptionOrders); i++) {
			static const int table[] = { 1, 2, 4, 8 };
			ASSERT(COUNT_OF(table) == COUNT_OF(m_receptionOrders));
			int tmp = server().send_step() / 4;
			tmp = (int)((double)(tmp + delta - 1) / delta) * delta;
			const int send_step = tmp * table[i];
			tmp = server().sense_body_step();
			tmp = (int)((double)(tmp + delta - 1) / delta) * delta;
			const int sense_body_step = tmp;
			int j=0;
			int timeSee = 0;
			int timeSenseBody = 0;
			do {
				timeSee -= delta;
				timeSenseBody -= delta;
				if(timeSenseBody <= -sense_body_step) {
					if(j >= m_historySize)
						break;
					m_receptionOrders[i][j++] = INFORMATION_SENSE_BODY;
					timeSenseBody = 0;
				}
				if(timeSee <= -send_step) {
					if(j >= m_historySize)
						break;
					m_receptionOrders[i][j++] = INFORMATION_SEE;
					timeSee = 0;
				}
			} while(timeSee != 0 || timeSenseBody != 0);
			m_receptionOrderSizes[i] = j;
		}
		
		m_indexHistoryNext = 0;
		for(i=0; i<m_historySize; i++)
			m_history[i] = INFORMATION_ERROR;
	}
	
	/**
	* 現在のスレッドで mutex をロックする。asObserver() で得られたオブザーバー
    * を呼び出すスレッド以外のスレッドからこのオブジェクトへアクセスする場合、
    * lock() と unlock() で排他制御すること。オブザーバーを呼び出すスレッドか 
    * らこのオブジェクトへアクセスする場合、書き込み時のみ排他制御すること。
	*/
	void lock() {
		m_mutex.lock();
	}
	/**
	* 現在のスレッドで mutex をアンロックする。asObserver() で得られたオブザー
    * バーを呼び出すスレッド以外のスレッドからこのオブジェクトへアクセスする場
    * 合、lock() と unlock() で排他制御すること。オブザーバーを呼び出すスレッ 
    * ドからこのオブジェクトへアクセスする場合、書き込み時のみ排他制御すること。
	*/
	void unlock() {
		m_mutex.unlock();
	}
	/**
	* オブザーバーを得る。オブザーバーは１つのスレッドからのみ呼び出されること。
    * オブザーバーを呼び出すスレッド以外のスレッドからこのオブジェクトへアクセ
    * スする場合、lock() と unlock() で排他制御すること。
	*/
	PlayerObserver& asObserver() {
		return *this;
	}
	/** シミュレーションを１ステップ進める。*/
	virtual void step() {
		m_ball.step();
		Players::iterator it = m_others.begin();
		for(; it != m_others.end(); it++)
			it->step();
		m_self.step();
	}
	
	/**
	* 審判の時計の指している時刻（Soccer Monitor に表示されているのと同じも   
    * の）を返す。
	*/
	int refereeTimer() const {
		return m_refereeTimer;
	}
	/**
	* 時刻（サーバへ接続してから経過したシミュレーションステップ数）を返す。
	*/
	int time() const {
		return m_time;
	}
	/**
	* 現在のプレイモードを返す。
	*/
	PlayMode playMode() const {
		return m_playMode;
	}
	/**
	* 味方チームの得点を返す。
	*/
	int scoreLeft() const {
		return m_scoreLeft;
	}
	/**
	* 敵チームの得点を返す。
	*/
	int scoreRight() const {
		return m_scoreRight;
	}
	
	/**
	* ボールを返す。
	*/
	const Ball& ball() const {
		return m_ball;
	}
	/**
	* 他のプレイヤーのリストを返す。
	*/
	const Players& others() const {
		return m_others;
	}
	/**
	* セルフを返す。
	*/
	const Self& self() const {
		return m_self;
	}
	/**
	* 直前の視覚センサーで捕らえられたフラグの位置のリストを返す。
	*/
	const Vectors& flags() const {
		return m_flags;
	}
};

template <class Ball, class Player, class Self>
bool Monoworld<Ball, Player, Self>::estimatePosition(const PlayerSee& si, const Angle& direction, Vector& result)
{
#if 0
	{
		// 左右４５度に近いフラグ２本から線を伸ばし、交点の座標を採用する方式
		ASSERT(direction.valid());
		double leftValue = DBL_MAX;
		const PlayerSeeObject* leftFlag = 0;
		double rightValue = DBL_MAX;
		const PlayerSeeObject* rightFlag = 0;
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->direction().valid() && o->flagId().valid()) {
				if(o->distance() == 0) {
					result = flagHelper.flagToPosition(o->flagId());
					return true;
				}
				double degrees = o->direction().degrees();
				if(abs(degrees - -45) < leftValue) {
					leftValue = abs(degrees - -45);
					leftFlag = o;
				}
				if(abs(degrees - +45) < rightValue) {
					rightValue = abs(degrees - +45);
					rightFlag = o;
				}
			}
		}
		if(leftFlag != 0 && (rightFlag->direction() - leftFlag->direction()).degrees() >= 30) {
			Vector left = flagHelper.flagToPosition(leftFlag->flagId());
			Vector right = flagHelper.flagToPosition(rightFlag->flagId());
			Vector leftToRight = right - left;
			Vector rightToLeft = -leftToRight;
			double l = leftToRight.norm();
			Angle alpha = (leftFlag->direction() + direction) - rightToLeft.angle();
			Angle beta = leftToRight.angle() - (rightFlag->direction() + direction);
			//ASSERT(-0.001 <= alpha.degrees() && alpha.degrees() <= 180.001);
			//ASSERT(-0.001 <= beta.degrees() && beta.degrees() <= 180.001);
			if(0 < alpha.degrees() && alpha.degrees() < 180 && 0 < beta.degrees() && beta.degrees() < 180) {
				// 次の連立方程式を解く
				// p*sin(alpha) == q*sin(beta)       -- [1]
				// p*cos(alpha) + q*cos(beta) == l   -- [2]
				//
				// [1] の左右を入れ換えたものを [3] とする。
				// [2]*sin(b) - [3]*cos(b): p*cos(a)*sin(b) == l*sin(b) - p*sin(a)*cos(b)
				//                          p == l * sin(b) / (cos(a)*sin(b) + sin(a)*cos(b))
				double sa = sin(alpha);
				double ca = cos(alpha);
				double sb = sin(beta);
				double cb = cos(beta);
				double p = l * sb / (ca * sb + sa * cb);
				ASSERT(p > 0);
				Vector delta = Vector(-p, leftFlag->direction() + direction);
				result = left + delta;
				return true;
			}
		}
		// この方法では計算できない。下へ続く。
	}
#endif
#if 0
	{
		// もっとも近いものから求める方式
		ASSERT(direction.valid());
		double minDistance = DBL_MAX;
		const PlayerSeeObject* minFlag = 0;
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->flagId().valid()) {
				if(o->distance() < minDistance) {
					minDistance = o->distance();
					minFlag = o;
				}
			}
		}
		if(minFlag == 0)
			return false;
		
		Vector toFlag = Vector(minDistance, direction + minFlag->direction());
		result = flagHelper.flagToPosition(minFlag->flagId()) - toFlag;
		return true;
	}
#else
	{
		// 重み付き平均を取る方式
		ASSERT(direction.valid());
		double sum = 0.0;
		Vector vector(0, 0);
		const PlayerSeeObject* const* flags = si.flags();
		for(; *flags; flags++) {
			const PlayerSeeObject* o = *flags;
			if(o->distance().valid() && o->flagId().valid()) {
				if(o->distance() == 0) {
					result = flagHelper.flagToPosition(o->flagId());
					return true;
				}
				ASSERT(abs(o->type()) == TYPE_FLAG);
				double distance = o->distance();
				Vector toFlag = Vector(distance, direction + o->direction());
				double coefficient = pow(0.9, distance);
				//double coefficient = pow(0.95, distance);
				Vector position = flagHelper.flagToPosition(o->flagId()) - toFlag;
				vector += position * coefficient;
				sum += coefficient;
			}
		}
		if(sum == 0.0)
			return false;
		
		result = vector / sum;
		return true;
	}
#endif
}

template <class Ball, class Player, class Self>
bool Monoworld<Ball, Player, Self>::estimateDirection(const PlayerSee& si, const Self& anticipatedSelf, Angle& result)
{
	const PlayerSeeObject* line;
	int edge;
	
	const PlayerSeeObject* const* lines = si.lines();
	if(!lines[0]) {
		// ラインが１本も見えなかったら、計算できない。
		return false;
	} else if(!lines[1]) {
		// １本だけ見えたら、プレイヤーはフィールドの中にいる。
		line = lines[0];
		edge = line->edge();
	} else if(!lines[2]) {
		// ２本見えたら、プレイヤーはフィールドの外にいる。
		line = lines[0];
		edge = -line->edge();
	} else {
		// ３本以上見えたら、サーバーの仕様が変更された。
		ASSERT(false);
		return false;
	}
	
	ASSERT(line);
	double degrees;
	double lineDegrees = line->direction().degrees();
	ASSERT(fabs(lineDegrees) <= 90.5);
	if(lineDegrees == 0) {
		if(!anticipatedSelf.sightDirection().valid())
			return false;
		double lastSightDegrees = anticipatedSelf.sightDirection().degrees();
		switch(edge) {
		case EDGE_TOP:
		case EDGE_BOTTOM:
			if(-90 <= lastSightDegrees && lastSightDegrees < +90)
				degrees = 0;
			else
				degrees = +180;
			break;
		case EDGE_RIGHT:
		case EDGE_LEFT:
			if(0 < lastSightDegrees)
				degrees = +90;
			else
				degrees = -90;
			break;
		default:
			ASSERT(false);
			return false;
		}
	} else if(fabs(lineDegrees) == 90.5) {
		switch(edge) {
		case EDGE_TOP:     degrees = -90;       break;
		case EDGE_RIGHT:   degrees = 0;         break;
		case EDGE_BOTTOM:  degrees = 90;        break;
		case EDGE_LEFT:    degrees = 180;       break;
		default:
			ASSERT(false);
			return false;
		}
	} else {
		switch(edge) {
		case EDGE_TOP:
			if(lineDegrees < 0)
				degrees = -lineDegrees - 180;
			else
				degrees = -lineDegrees;
			break;
		case EDGE_BOTTOM:
			if(lineDegrees < 0)
				degrees = -lineDegrees;
			else
				degrees = 180 - lineDegrees;
			break;
		case EDGE_LEFT:
			if(lineDegrees < 0)
				degrees = 90 - lineDegrees;
			else
				degrees = -90 - lineDegrees;
			break;
		case EDGE_RIGHT:
			if(lineDegrees < 0)
				degrees = -90 - lineDegrees;
			else
				degrees = 90 - lineDegrees;
			break;
		default:
			ASSERT(false);
			return false;
		}
	}
	result = Angle(degrees);
	
	return true;
}

template <class Ball, class Player, class Self>
void Monoworld<Ball, Player, Self>::received(const PlayerSee& si)
{
	tryToStep(INFORMATION_SEE, si.refereeTimer());
	updateSelf(si);
	updateBall(si);
	updateFlags(si);
	
	// others の残像の削除
	const double halfWidthDegrees = self().viewWidthDegrees() / 2;
	Players::iterator it = m_others.begin();
	while(it != m_others.end()) {
		bool remove = true;
		const Player& p = *it;
		if(p.position().valid()) {
			Vector toPlayer = p.position() - self().position();
			if(toPlayer.norm() > server().visible_distance()) {
				Angle angle = toPlayer.angle() - self().sightDirection();
				if(abs(angle).degrees() >= halfWidthDegrees)
					remove = false;
			}
		} else {
			remove = true;
		}
		if(remove)
			it = m_others.erase(it);
		else
			it++;
	}
	
	// others の更新
	const PlayerSeeObject* const* players = si.players();
	for(; *players; players++) {
		m_others.push_front(Player());
		m_others.front().initialize(self(), m_time, **players);
	}
}

/////////////////////////////////////////////////////////////////////////////

#endif // } !defined(MONOWORLD_HXX__INCLUDED)
