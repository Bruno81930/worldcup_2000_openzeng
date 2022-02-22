// Monobrain.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MONOBRAIN_HXX__INCLUDED) // {
#define MONOBRAIN_HXX__INCLUDED

#include "Premise.hxx"
#include "Synchronizer.hxx"
#include "PlayerClient.hxx"
#include "Window.hxx"
#include "Screen.hxx"

/////////////////////////////////////////////////////////////////////////////
// Monobrain

/**
* （単一人格型）ブレイン。構築後、connect あるいは reconnect メンバ関数でサー 
* バへ接続し、go メンバ関数で始動する。act メンバ関数に、プレイヤーを操作する 
* アルゴリズムが記述される。現在のバージョンでは、クライアントの思考時間と、ク
* ライアントサーバ間のメッセージの往復にかかる時間との和が、１シミュレータサイ
* クルに対して十分小さいことを仮定している。
* 
* 　go メンバ関数は、サーバとの接続が切断されない限り、act メンバ関数を呼び出 
* しつづける（act メンバ関数が例外を返した場合でも、サーバとの接続が切断された
* ことを表す例外でない限り、act メンバ関数を呼び出しつづける）。ただし、act メ
* ンバ関数がコンシューマ（１シミュレーションサイクルに１回しか実行できないコマ
* ンドをコンシューマと呼ぶことにする）を発行せずに終了した場合、ビジーループを
* 防止するために、rest メンバ関数を１回呼び出す。
* 
* 　act メンバ関数中では、world メンバ関数によって「ワールドモデル」へアクセス
* することができる。「ワールドモデル」は act メンバ関数が現在選択しようとして 
* いるコマンドが実行される直前の世界の状態を表す（将来のバージョンでは、サーバ
* との通信に時間がかかる場合、コマンドが実際に受信される時刻の直前の世界を予測
* したワールドモデルが提供されるかもしれない）。
* 
* 　act メンバ関数では、コマンドに対応するメンバ関数を用いて、コマンドをサーバ
* へ送信する。コマンド／コンシューマ／非コンシューマに対応するメンバ関数をそれ
* ぞれ、コマンド関数／コンシューマ関数／非コンシューマ関数と呼ぶことにする。コ
* マンド関数はサーバと同期をとって動作する。コマンド関数の引き数は、サーバのマ
* ニュアルで定められる範囲内に無ければならない（範囲内にない場合 assert で弾か
* れる）。
* 　コンシューマ関数が終了した時点で、次のシミュレーションサイクルにおける行動
* を思考する用意が整っていることが保証される。すなわち、ワールドモデルが更新さ
* れ（必要な情報が得られるまで、関数は終了しない）、次に呼び出すコンシューマ関
* 数によるコマンドは、次のシミュレーションサイクルで実行される。
* 　非コンシューマ関数を呼び出した場合、次のコンシューマ関数の呼び出しによって
* 送信されるコンシューマが、サーバによって受信されるのと同じシミュレーションサ
* イクルにおいて、その非コンシューマであるコマンドをサーバが受信するように送信
* される（したがって、非コンシューマ・コンシューマの送信順序は保証されない）。
* 現在のバージョンでは、非コンシューマ関数が呼び出されてから、次のコンシューマ
* 関数が呼び出されるまで、ワールドモデルは不定とする。（非コンシューマを反映す
* べきかどうか結論が出ていないため）。
* 　このシステムでは「何もしない」ことを明示するために、コンシューマ rest が仮 
* 想的に用意されている（実際にはサーバへコマンドを送信しないが、このシステムの
* 上では rest コマンドが存在する）。
* 
* 　前提を設定することができる。コマンド関数では、実行の前後で前提がチェックさ
* れ、崩れたものがある場合、PremiseException が throw される。
* 
* 　addObserver によって、サーバとの送受信を監視するオブジェクトを用意すること
* ができる（が、とりあえず今のところ、通知のタイミングや順番を保証しない仕様に
* しているので、バージョンに依存しないプログラムはほとんど書けない）。
* 
* わかりにくい説明ですいません。
* 
* @param	ColorWorld
*				ワールドモデルクラス。
*/
class White;
template<class ColorWorld = White>
class Monobrain : public PremiseOwner, public PlayerClient
{
private:
	Monobrain(const Monobrain&);
	Monobrain& operator= (const Monobrain&);
	bool operator== (const Monobrain&) const;

public:
	typedef ColorWorld World;
	typedef typename ColorWorld::Ball Ball;
	typedef typename ColorWorld::Player Player;
	typedef typename ColorWorld::Players Players;
	typedef typename ColorWorld::Self Self;
	
private:
	Synchronizer m_synchronizer;
	class Observer : public PlayerObserver
	{
		const Monobrain* m_brain;
	public:
		Observer(const Monobrain* brain) {
			m_brain = brain;
		}
		virtual void intoWaiting() {
			if(m_brain->m_window) {
				Vector toBall = m_brain->ball().position() - m_brain->self().position();
				m_brain->m_window->update(m_brain, toBall.norm());
			}
		}
	};
	friend class Observer;
	Observer m_observer;
	
protected:
	World m_world;

	/**
	* Soccer Server へ init コマンドによって接続する。最初にキーパーとしての登
    * 録を試み、失敗したら通常のプレイヤーとして登録する。
	* @param	hostName
	*				サーバのホスト名。
	* @param	port
	*				サーバのポート番号。
	* @param	teamName
	*				チームの名前。
	* @return	成功したら true、失敗したら false。
	*/
	bool connect(const char* hostName, int port, const char* teamName) {
		return m_synchronizer.connect(hostName, port, teamName);
	}
	/**
	* Soccer Server へ reconnect コマンドによって接続する。最初にキーパーとしての登
    * 録を試み、失敗したら通常のプレイヤーとして登録する。
	* @param	hostName
	*				サーバのホスト名。
	* @param	port
	*				サーバのポート番号。
	* @param	teamName
	*				チームの名前。
	* @param	uniformNumber
	*				コントロールするプレイヤーの背番号（0 以上 10 未満の値）。
	* @return	成功したら true、失敗したら false。
	*/
	bool reconnect(const char* hostName, int port, const char* teamName, int uniformNumber) {
		return m_synchronizer.reconnect(hostName, port, teamName, uniformNumber);
	}

	/** ブレインを実行する。サーバとの接続が切れるまで終了しない。*/
	virtual void go();
	/**
	* 派生クラスで定義されるブレインの思考ルーチン。このルーチンの中では、
	* ・addObserver と removeObserver
	* ・addPremise、removePremise と、checkPremises
	* ・dash や turn などのサーバへコマンドを送信するメンバ関数
	* ・world や self などの public メンバ関数
	* を呼び出すことができる。
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	virtual void act() = 0;

	/** 通信を監視するオブザーバーを追加する。*/
	void addObserver(PlayerObserver* observer) {
		m_synchronizer.addObserver(observer);
	}
	/** オブザーバーを削除する。*/
	void removeObserver(PlayerObserver* observer) {
		m_synchronizer.removeObserver(observer);
	}

	/* コンシューマ関数が終了し、制御が呼びだし側に戻る直前に呼び出される。*/
	virtual void steped() { }

	/**
	* dash する（コンシューマ）。
	* @param	power
	*				パワー
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void dash(double power) {
		checkPremises();
		if(power == 0)
			return;
		m_synchronizer.dash(power);
		checkPremises();
		steped();
	}
	/**
	* dash する（コンシューマ）。
	* @param	length
	*				ダッシュする距離
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void dashTo(double length) {
		checkPremises();
		if(length == 0)
			return;
		length = length / (1.0 + server().player_decay());	// ２サイクルで到着
		double power = length / self().effort() / server().dash_power_rate();
		power = suppress(0.0, power, server().maxpower());
		m_synchronizer.dash(power);
		checkPremises();
		steped();
	}
	/**
	* turn する（コンシューマ）。
	* @param	angle
	*				自分の体の向きを中心とした、回転後の方向を表す相対角度。た
	*				だし、１サイクルで回転できる角度より大きい場合、可能な限り
	*				の回転を行う。
	* @param	block
	*				回転が反映された see 情報を受信するまで、関数から戻らない？
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void turn(const Angle& angle, bool block = false) {
		checkPremises();
		if(angle == Angle(0))
			return;
		double degrees = angle.degrees();
		double moment = degrees * (1.0 + server().inertia_moment() * self().safeVelocity().norm());
		moment = suppress(server().minmoment(), moment, server().maxmoment());
		m_synchronizer.turn(moment);
		checkPremises();
		int last = world().time();
		if(block) {
			while(self().time() < last) {
				m_synchronizer.rest();
				checkPremises();
			}
		}
		steped();
	}
	/**
	* turn する（コンシューマ）。
	* @param	direction
	*				回転後の方向を表す絶対方向。そちらの方向を向くまで、終了し
	*				ないので、この関数の呼出しで、１サイクル以上経過する場合が
	*				ある。
	* @param	block
	*				回転が反映された see 情報を受信するまで、関数から戻らない？
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void turnTo(Angle direction, bool block = false) {
		checkPremises();
		int last = world().time();
		for(;;) {
			double degrees = (direction - self().bodyDirection()).degrees();
			if(degrees == 0)
				break;
			double moment = degrees * (1.0 + server().inertia_moment() * self().safeVelocity().norm());
			double rounded = suppress(server().minmoment(), moment, server().maxmoment());
			m_synchronizer.turn(rounded);
			last = world().time();
			if(rounded == moment)
				break;
		}
		checkPremises();
		if(block) {
			while(self().time() < last) {
				m_synchronizer.rest();
				checkPremises();
			}
		}
		steped();
	}
	/**
	* kick する（コンシューマ）。
	* @param	power
	*				パワー
	* @param	direction
	*				方向
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void kick(double power, const Angle& direction) {
		checkPremises();
		m_synchronizer.kick(power, direction);
		checkPremises();
		steped();
	}
	/**
	* kick する（コンシューマ）。
	* @param	acceleration
	*				ボールへ与える絶対加速度
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void kick(Vector acceleration) {
		checkPremises();
		double norm = acceleration.norm();
		if(norm == 0)
			return;
		Angle direction = acceleration.angle() - self().bodyDirection();
		Vector toBall = ball().position() - self().position();
		double distanceToBall = toBall.norm();
		Angle directionToBall = ((distanceToBall == 0) ? Angle(0) : toBall.angle());
		double power = norm / server().kick_power_rate();
		power /= 1.0 - 0.25 * abs(directionToBall - self().bodyDirection()).degrees()/180
			- 0.25 * (distanceToBall - server().player_size() - server().ball_size()) / server().kickable_margin();
		power = suppress(0.0, power, server().maxpower());
		m_synchronizer.kick(power, direction);
		checkPremises();
		steped();
	}
	/**
	* catch する（コンシューマ）。
	* @param	direction
	*				方向
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void catchBall(const Angle& direction) {
		checkPremises();
		m_synchronizer.catchBall(direction);
		checkPremises();
		//SkickTsteped();
	}
	/**
	* move する（コンシューマ）。
	* @param	position
	*				移動先
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void move(const Vector& position) {
		checkPremises();
		m_synchronizer.move(position);
		checkPremises();
		steped();
	}
	/**
	* 何もしない（コンシューマ）。
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void rest() {
		checkPremises();
		m_synchronizer.rest();
		checkPremises();
		steped();
	}
	
	/**
	* 首を回す（非コンシューマ）。１シミュレーションサイクルに、１回しか首を回
    * すことはできない。
	* @param	angle
	*				首を回す角度（現在の角度に対する相対角度）。回した結果、首
	*				の体に対する角度が [server().minneckang(), server().      
	*				maxneckang()] の範囲外になってはならない。
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void turnNeck(const Angle& angle) {
		checkPremises();
		m_synchronizer.turnNeck(angle);
	}
	/**
	* 首を回す（非コンシューマ）。１シミュレーションサイクルに、１回しか首を回
    * すことはできない。
	* @param	angle
	*				回した後にそうなるべき、首の体に対する相対角度。
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	void turnNeckTo(Angle angle) {
		checkPremises();
		angle = Angle(suppress(server().minneckang()+0.1, angle.degrees(), server().maxneckang()-0.1));
		ASSERT(server().minneckang() <= angle.degrees() && angle.degrees() <= server().maxneckang());
		Angle relative = angle - self().neckAngle();
		relative = Angle(suppress(server().minneckmoment()+0.1, relative.degrees(), server().maxneckmoment()-0.1));
		ASSERT(server().minneckmoment() <= relative.degrees() && relative.degrees() <= server().maxneckmoment());
		m_synchronizer.turnNeck(relative);
	}
	
	/**
	* say する（非コンシューマ）。
	* @param	i
	*				整数型の変数で、叫ばれるメッセージ。
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	template<class T> void say(T i)
	{
		m_synchronizer.say(i);
	}
	/**
	* say する（非コンシューマ）。
	* @param	i
	*				整数型の変数で、叫ばれるメッセージ。
	* @param	bits
	*				i の下位 bits ビットを叫ぶ。
	* @throws	PremiseException
	*				前提が崩れた。
	*/
	template<class T> void say(T i, int bits)
	{
		m_synchronizer.say(i, bits);
	}
	
	/**
	* ビューモードを変更する（非コンシューマ）。
	*/
	void changeView(ViewWidth width, ViewQuality quality) {
		if(self().viewWidth() != width || self().viewQuality() != quality)
			m_synchronizer.changeView(width, quality);
	}
	
public:
	virtual ~Monobrain() {
	}
	Monobrain(int key) : m_synchronizer(key), m_observer(this) {
		m_synchronizer.addObserver(&m_world.asObserver());
		m_synchronizer.addObserver(&m_observer);
	}

	virtual void paintClient(Screen& screen);
	
	/**
	* 受信したメッセージの数。
	*/
	int countReceived() const {
		return m_synchronizer.countReceived();
	}
	/**
	* ワールドモデルを返す。デフォルトでは White オブジェクトを返す。
	*/
	const World& world() const {
		return m_world;
	}
	/**
	* world().ball() を返す。デフォルトでは WhiteBall オブジェクトを返す。
	*/
	const Ball& ball() const {
		return world().ball();
	}
	/**
	* world().others() を返す。デフォルトでは WhitePlayer オブジェクトを返す。
	*/
	const Players& others() const {
		return world().others();
	}
	/**
	* world().self() を返す。デフォルトでは WhiteSelf オブジェクトを返す。
	*/
	const Self& self() const {
		return world().self();
	}
};

template<class World>
void Monobrain<World>::go()
{
	int count = countReceived();
	for(;;) {
#if NEVER_THROW
#  error
#else
		try {
			if(count == countReceived())
				rest();	// ビジーループ防止
			count = countReceived();
			act();
		} catch(PremiseException) {
		} catch(ConnectionException& e) {
			if(strcmp(e.what(), "time-out") == 0)
				return;
			report("error: ConnectionException '%s'\n", (const char*)e.what());
		} catch(std::exception& e) {
			report("error: exception '%s'\n", (const char*)e.what());
#ifdef NDEBUG
		} catch(...) {
			report("error: Unknown\n");
#endif
		}
#endif
	}
}

template<class World>
void Monobrain<World>::paintClient(Screen& screen)
{
	screen.paintField();
	m_world.lock();
	{
		Screen& s = screen;

		// 情報を表示
		char buffer[1024];
		int len = sprintf(buffer,
			"#%d stamina:%f effort:%f  [%s]  neckAngle:%f  time:%d (world/self/ball:/%d/%d/%d)",
			self().uniformNumber(), self().stamina(), self().effort(),
			playModeHelper.toString(world().playMode()), (double)self().neckAngle().degrees(),
			(int)world().refereeTimer(), (int)world().time(), (int)self().time(), (int)ball().time());
		ASSERT(len < 1023);
		if(ball().velocity().valid()) {
			len = sprintf(buffer+len,
				"  ball_speed:%f",
				ball().velocity().norm());
			ASSERT(len < 1023);
		}
		s.write(buffer);
		
		// ビューコーンなどを表示
		s.color(s.colorGreen());
		s.circle(self().position().x(), self().position().y(), server().player_size());
		s.circle(self().position().x(), self().position().y(), server().player_size() + server().kickable_margin());
		double x = self().position().x();
		double y = self().position().y();
		s.line(x, y, x + server().CENTER_CIRCLE_R() * cos(self().bodyDirection().radians()),
			y + server().CENTER_CIRCLE_R() * sin(self().bodyDirection().radians()));
		double widthRadians = self().viewWidthDegrees() * (PI/180.0);
		double startRadians = self().sightDirection().radians() - widthRadians/2;
		double endRadians = self().sightDirection().radians() + widthRadians/2;
		s.arc(x, y, server().visible_distance(), endRadians, startRadians);
		s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(startRadians), y + 2*server().PITCH_LENGTH() * sin(startRadians));
		s.line(x, y, x + 2*server().PITCH_LENGTH() * cos(endRadians), y + 2*server().PITCH_LENGTH() * sin(endRadians));
		s.arc(x, y, server().UNUM_FAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().UNUM_TOOFAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().TEAM_FAR_LENGTH(), startRadians, endRadians);
		s.arc(x, y, server().TEAM_TOOFAR_LENGTH(), startRadians, endRadians);

		// フラグを表示
		{
			s.color(s.colorFlag());
			typename World::Vectors::const_iterator it = world().flags().begin();
			for(; it != world().flags().end(); it++) {
				const double d = 0.8;
				const Vector& v = *it;
				s.circle(v.x(), v.y(), d);
			}
		}
		
		// プレイヤーを表示
		{
			typename Players::const_iterator it = others().begin();
			for(; it != others().end(); it++) {
				const Player& p = *it;
				int color = s.colorYellow();
				if(p.team().valid()) {
					switch(p.team()) {
					case SIDE_LEFT:
						color = s.colorGreen();
						break;
					case SIDE_RIGHT:
						color = s.colorRed();
						break;
					default:
						ASSERT(false);
					}
				}
				s.color(color);
				if(p.uniformNumber().valid()) {
					s.circle(p.position().x(), p.position().y(), server().player_size());
				} else {
					s.circleFill(p.position().x(), p.position().y(), server().player_size());
				}
			}
		}

		// ボールを表示
		{
			const double d = 0.5;
			if(ball().time() < world().time())
				s.color(s.colorBallAnticipated());
			else
				s.color(s.colorBall());
			const Vector& v = ball().position();
			s.circle(v.x(), v.y(), server().ball_size());
			s.line(v.x() - d, v.y(), v.x() + d, v.y());
			s.line(v.x(), v.y() - d, v.x(), v.y() + d);
		}
	}
	m_world.unlock();
}

/////////////////////////////////////////////////////////////////////////////

#endif // } !defined(MONOBRAIN_HXX__INCLUDED)
