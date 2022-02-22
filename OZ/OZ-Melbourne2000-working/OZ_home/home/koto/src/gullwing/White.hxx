// White.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(WHITE_HXX__INCLUDED) // {
#define WHITE_HXX__INCLUDED

#include "basic.hxx"
#include "PlayerObserver.hxx"
#include "Mutex.hxx"
#include "Monoworld.hxx"
class WhiteSelf;

/////////////////////////////////////////////////////////////////////////////
// WhiteObject

/**
* White ワールドモデルオブジェクト。
*/
class WhiteObject
{
protected:
	int m_time;
	Vector m_position;
	Vector m_velocity;
	Vector m_safeVelocity;
	Vector m_acceleration;
public:
	/*dummy*/
	virtual double size() const = 0;
	virtual double decay() const = 0;
	virtual double weight() const = 0;
	virtual double maxSpeed() const = 0;
	/**
	* このオブジェクトを、最後に目視した時刻を返す。WhiteSelf の場合、最後に  
    * see 情報を受け取った時刻を返す。
	*/
	int time() const;
	/** オブジェクトの位置を返す。*/
	const Vector& position() const;
	/** オブジェクトの速度を返す。*/
	const Vector& velocity() const;
	/** オブジェクトの速度を返す。速度が無効の場合、以前の速度から予測される値、もしくは Vector(0, 0) を返す。*/
	Vector safeVelocity() const;
	/** オブジェクトの加速度を返す。*/
	const Vector& acceleration() const;
	/** オブジェクトの加速度を返す。加速度が無効の場合 Vector(0, 0) を返す。*/
	Vector safeAcceleration() const;
	
public:
	virtual ~WhiteObject();
	
protected:
	/*dummy*/
	/**
	* 初期化する。
	*/
	void initialize(int time, const Vector& position,
		const Vector& velocity, const Vector& acceleration);
	void initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	/**
	* o で与えられた情報に一致するように、なるべく現在の状態を維持しつつ更新する。
	*/
	void update(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	/**
	* シミュレーションを１ステップ実行した後の状態へ更新する。
	*/
	void step();
};

/////////////////////////////////////////////////////////////////////////////
// WhiteBall

/**
* White ワールドモデルボール。bodyDirection と neckAngle は常に無効である。
*/
class WhiteBall : public WhiteObject {
public:
	virtual double size() const;
	virtual double decay() const;
	virtual double weight() const;
	virtual double maxSpeed() const;
	
	void initialize(int time = -1,
		const Vector& position = Vector(0, 0),
		const Vector& velocity = Vector::invalidValue(),
		const Vector& acceleration = Vector::invalidValue());
	void initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	void update(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	void average(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	void step();
	
	void kicked(const WhiteSelf& self, double power, Angle direction);
};

/////////////////////////////////////////////////////////////////////////////
// WhitePlayer

/**
* White ワールドモデルプレイヤー。
*/
class WhitePlayer : public WhiteObject {
protected:
	Angle m_bodyDirection;
	Angle m_neckAngle;
	Side m_team;
	Integer m_uniformNumber;
public:
	virtual double size() const;
	virtual double decay() const;
	virtual double weight() const;
	virtual double maxSpeed() const;
	
	/** オブジェクトの体の向きを返す。*/
	const Angle& bodyDirection() const;
	/** オブジェクトの頭が、体の向きに対する相対的な角度を返す。*/
	const Angle& neckAngle() const;
	/** オブジェクトの視覚センサーの絶対方向を返す。*/
	Angle sightDirection() const;
	/** 所属チームを返す。味方の場合 SIDE_LEFT、敵の場合 SIDE_RIGHT。*/
	Side team() const;
	/** 背番号を返す。*/
	Integer uniformNumber() const;
	
	void initialize(Side team, const Integer& uniformNumber,
		int time, const Vector& position, const Vector& velocity,
		const Vector& acceleration, Angle bodyDirection, Angle neckAngle);
	void initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	void update(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	void step();
};

/////////////////////////////////////////////////////////////////////////////
// WhiteSelf

/**
* White ワールドモデルセルフ。全てのメンバ関数は、無効値を返さない。
*/
class WhiteSelf : public WhitePlayer {
protected:
	bool m_goalie;
	ViewWidth m_viewWidth;
	ViewQuality m_viewQuality;
	double m_stamina;
	double m_effort;
	Angle m_turnAngle;		// 次の step で体を回転する角度
	Angle m_neckTurnAngle;	// 次の step で首を回転する角度
	int m_timeLastCatch;
public:
	/** ゴールキーパー？ */
	bool isGoalie() const;
	/** ビューモードの質を返す */
	ViewQuality viewQuality() const;
	/** ビューモードの幅を返す */
	ViewWidth viewWidth() const;
	/** ビューモードの幅を度単位で返す */
	double viewWidthDegrees() const;
	/** スタミナを返す */
	double stamina() const;
	/** エフォートを返す */
	double effort() const;
	/** 最後に catch を試みた時刻を返す */
	double timeLastCatch() const;
	
	void initialize(int uniformNumber, bool goalie, int time,
		const Vector& position = Vector(0, 0),	// 全て常時 valid。
		const Vector& velocity = Vector(0, 0),
		const Vector& acceleration = Vector(0, 0),
		Angle bodyDirection = Angle(0),
		Angle neckAngle = Angle(0));
	void step();
	
	void update(const PlayerSenseBody& sbi);
	void updateSightDirection(Angle direction);		// neckAngle を変化させずに bodyDirection を更新する
	void updatePosition(Vector position);
	void updateVelocity(Vector velocity);
	void updateTime(int time);
	
	void turned(double moment);
	void dashed(double power);
	void neckTurned(Angle angle);
	void ballCaught(int time, Angle direction);
	void moved(Vector position);
	void viewChanged(ViewWidth width, ViewQuality quality);
};

/////////////////////////////////////////////////////////////////////////////
// White

/**
* ワールドモデル。なお、ワールドモデルは複数用意される可能性があるため、色の名
* 前を形容詞として使用することで区別する。
*/
class White : public Monoworld<WhiteBall, WhitePlayer, WhiteSelf>
{
private:
	White(const White&);
	White& operator= (const White&);
	bool operator== (const White&) const;
public:
	White();
};

/*
class White : protected PlayerObserver
{
public:
	typedef const WhitePlayer* const_iterator;
	typedef WhitePlayer* iterator;
	
private:
	Mutex m_mutex;
	int m_time;
	PlayMode m_playMode;
	WhiteSelf m_self;
	WhiteBall m_ball;
	WhitePlayer m_players[MAX_PLAYER * 5];
	int m_countOfPlayers;
	Vector m_flags[FLAG_MAZ + 1];
	
protected:
	iterator begin();
	iterator end();
	static bool estimatePosition(const PlayerSee& si, const Angle& direction, Vector& result);
	static bool estimateDirection(const PlayerSee& si, const WhiteSelf& lastSelf, Angle& result);
	
public:
	virtual ~White();
	White();
	
	void lock();
	void unlock();

	PlayerObserver& asObserver();
	
	int time() const;
	PlayMode playMode() const;
	const WhiteSelf& self() const;
	const WhiteBall& ball() const;
	const_iterator begin() const;
	const_iterator end() const;
	const Vector* flags() const;

	void step();
};*/

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "White.inl"
#endif

#endif // } !defined(WHITE_HXX__INCLUDED)
