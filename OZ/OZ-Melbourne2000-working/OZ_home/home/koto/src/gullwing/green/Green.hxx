// Green.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(GREEN_HXX__INCLUDED) // {
#define GREEN_HXX__INCLUDED

#include "../White.hxx"
#include "Config.hxx"

/////////////////////////////////////////////////////////////////////////////
// Green

typedef WhiteObject GreenObject;
//typedef WhiteBall GreenBall;
//typedef WhitePlayer GreenPlayer;
typedef WhiteSelf GreenSelf;

class GreenBall : public WhiteBall {
public:
	void setVelocity(const Vector& vel);
	void average(const GreenSelf& self, int time, const PlayerSeeObject& o);
	void step();
	void step(const GreenSelf& self);
};

class GreenPlayer : public WhitePlayer
{
#if LEARNING
public:
	// inspiration で知った正しいチームと背番号
	Side m_inspirationTeam;
	int m_inspirationUniformNumber;
	void initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o) {
		WhitePlayer::initialize(self, time, o);
		m_inspirationTeam = o.m_inspirationTeam;
		m_inspirationUniformNumber = o.m_inspirationUniformNumber;
	}
	void update(const WhiteSelf& self, int time, const PlayerSeeObject& o) {
		WhitePlayer::update(self, time, o);
		m_inspirationTeam = o.m_inspirationTeam;
		m_inspirationUniformNumber = o.m_inspirationUniformNumber;
	}
#endif
};

class Green : public Monoworld<GreenBall, GreenPlayer, GreenSelf> {
	Green(const Green&);
	Green& operator= (const Green&);
	bool operator== (const Green&) const;
#if LEARNING
public:
	bool m_inspiration;
	CoachSee m_inspirationSee;
#endif

protected:
	Config* m_config;

public:
	Green();
	Config& config();

	void setConfig(Config* config);

#if LEARNING
	virtual void received(const PlayerSenseBody& sbi);
#endif
	virtual void received(const PlayerSee& si);
	virtual void updateBall(const PlayerSee& si);
	virtual void step();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Green.inl"
#endif

#endif // } !defined(GREEN_HXX__INCLUDED)
