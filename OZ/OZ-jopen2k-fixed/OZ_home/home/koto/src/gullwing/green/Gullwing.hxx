// Gullwing.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(GULLWING_HXX__INCLUDED) // {
#define GULLWING_HXX__INCLUDED

#include "../Monobrain.hxx"
#include "../PlayerClient.hxx"
#include "../Random.hxx"
#include "Config.hxx"
#include "Green.hxx"

/////////////////////////////////////////////////////////////////////////////
// GullwingPremise

class Gullwing;
class GullwingPremise : public Premise
{
protected:
	Gullwing* m_brain;
public:
	GullwingPremise(Gullwing* brain);
};

/////////////////////////////////////////////////////////////////////////////
// Passwork

struct PlayerState {
	const WhitePlayer* player;
	Vector position;
	int time;	// 時刻 time に、position の位置にいる
};
typedef std::vector<PlayerState> PlayerStates;

struct Passwork
{
	Angle startDirection;
	double startSpeed;

	PlayerStates playerStates;
	Vector ballPosition;
	int time;
	bool closed;
	
	bool shoot;
	int catcher;
	Vector firstCatch;
	double enemy;
#if WITH_WINDOW
	typedef std::list<Vector> Vectors;
	Vectors traces;
#endif

	double score() const {
		if(shoot)
			return server().PITCH_LENGTH() * 2 + enemy;
		double x = ballPosition.x();
		if(enemy < server().CENTER_CIRCLE_R())
			x -= 2 * (server().CENTER_CIRCLE_R() - enemy);
		return x;
	}
	bool operator> (const Passwork& lhs) const {
		return score() > lhs.score();
	}
};

/////////////////////////////////////////////////////////////////////////////
// Gullwing

const short SAY_MOVE = 0;

class Gullwing : public Monobrain<Green>, public PlayerObserver
{
private:
	Gullwing(const Gullwing&);
	Gullwing& operator= (const Gullwing&);
	bool operator== (const Gullwing&) const;

protected:
	double m_enemyOffside;
	double m_ourOffside;
	Vector m_toBall;
	double m_distanceToBall;
	Angle m_directionToBall;
	Vector m_toNextBall;
	double m_distanceToNextBall;
	Angle m_directionToNextBall;

public:
	virtual ~Gullwing();
	Gullwing(int key, const std::map<std::string, std::string>& parameters);

	virtual void received(const PlayerHear& hi, int key);
	
	virtual void paintClient(Screen& screen);
	virtual void go();
	virtual void act();

	double enemyOffside() const;
	double ourOffside() const;
	const Vector& toBall() const;
	double distanceToBall() const;
	const Angle& directionToBall() const;
	const Vector& toNextBall() const;
	double distanceToNextBall() const;
	const Angle& directionToNextBall() const;
	virtual void steped();

	enum Mode {
		MODE_NORMAL, 
			MODE_SETPLAYL,
			MODE_SETPLAYR,
	};
    Mode m_mode;
	int m_timePlayModeChanged;
	int m_timeCaught;
	Random m_random;
	Config m_config;
	int m_freeCount;
	const Config& config();
	double powerLimit(double stamina, bool forward, double dash) const;
	double cyclesDashableWithFullPower() const;
	double reachableByDash() const;
	double getPositionValue(Vector position) const;

	// シミュレーションを行う。ボールを捕捉したプレイヤーの添え字、-1(だれも捕捉しない)、
	// -2(ボールが外に出た)、もしくは -3(シュートが決まった)を返す。
	int simulate(const PlayerStates& players, Vector& ballPosition, Vector& ballVelocity, int& time, double& nearestEnemy);

	Vector m_moveTarget;	// valid ならば、そこまで行かなければならない
	int m_moveTimeLimit;

#if WITH_WINDOW
	typedef std::list<Vector> Vectors;
	Vectors m_traces;
#endif

	void passTo(const Vector& target);	// target へパスする
	void kickTo(const Angle& direction);
	void catchIn(const Angle& direction);

	void movable();
	void goalieCatching();
	void normal();
	void ballNear();
	void ballFar();
	void free();
	
	void dashTo(Vector target, double dash);
	void kickTwiceTo(double initialSpeed, Angle directionToTarget);
	void turnBallTo(Vector toTarget);		// 指定の場所まで体のまわりを回して運ぶ
	void findBall();
	void seeAround();

	static bool isInPitch(const Vector& v);
	static bool isInMyPenaltyArea(const Vector& v);
	static bool isInEnemyPenaltyArea(const Vector& v);
	static bool isInMyGoalArea(const Vector& v);
	static bool isInMyHalfGoalArea(const Vector& v);

	// 以下、ワークエリア
	enum { LEFT=0, RIGHT=1 };
#undef NEAR
#undef FAR
	enum Distance { NEAR=0, FAR=1, TOOFAR=2 };
	enum { sizeWorkArea = 360 };
	double m_minOthers[2][3][sizeWorkArea+1];
	double m_maxOthers[2][3][sizeWorkArea+1];
	double m_minOthersMaster[2][3][sizeWorkArea+1];
	double m_maxOthersMaster[2][3][sizeWorkArea+1];
	double minOthers(Side side, Distance distance, int direction) const;
	double maxOthers(Side side, Distance distance, int direction) const;
	double minOthers(Side side, Distance distance) const;
	double maxOthers(Side side, Distance distance) const;
	double m_lineOffside;
	double lineOffside() const;
	void initializeWorkArea();	// ワールドモデルをもとに、ワークエリアを初期化する
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Gullwing.inl"
#endif

#endif // } !defined(GULLWING_HXX__INCLUDED)
