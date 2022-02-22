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
* White $B%o!<%k%I%b%G%k%*%V%8%'%/%H!#(B
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
	* $B$3$N%*%V%8%'%/%H$r!":G8e$KL\;k$7$?;~9o$rJV$9!#(BWhiteSelf $B$N>l9g!":G8e$K(B  
    * see $B>pJs$r<u$1<h$C$?;~9o$rJV$9!#(B
	*/
	int time() const;
	/** $B%*%V%8%'%/%H$N0LCV$rJV$9!#(B*/
	const Vector& position() const;
	/** $B%*%V%8%'%/%H$NB.EY$rJV$9!#(B*/
	const Vector& velocity() const;
	/** $B%*%V%8%'%/%H$NB.EY$rJV$9!#B.EY$,L58z$N>l9g!"0JA0$NB.EY$+$iM=B,$5$l$kCM!"$b$7$/$O(B Vector(0, 0) $B$rJV$9!#(B*/
	Vector safeVelocity() const;
	/** $B%*%V%8%'%/%H$N2CB.EY$rJV$9!#(B*/
	const Vector& acceleration() const;
	/** $B%*%V%8%'%/%H$N2CB.EY$rJV$9!#2CB.EY$,L58z$N>l9g(B Vector(0, 0) $B$rJV$9!#(B*/
	Vector safeAcceleration() const;
	
public:
	virtual ~WhiteObject();
	
protected:
	/*dummy*/
	/**
	* $B=i4|2=$9$k!#(B
	*/
	void initialize(int time, const Vector& position,
		const Vector& velocity, const Vector& acceleration);
	void initialize(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	/**
	* o $B$GM?$($i$l$?>pJs$K0lCW$9$k$h$&$K!"$J$k$Y$/8=:_$N>uBV$r0];}$7$D$D99?7$9$k!#(B
	*/
	void update(const WhiteSelf& self, int time, const PlayerSeeObject& o);
	/**
	* $B%7%_%e%l!<%7%g%s$r#1%9%F%C%W<B9T$7$?8e$N>uBV$X99?7$9$k!#(B
	*/
	void step();
};

/////////////////////////////////////////////////////////////////////////////
// WhiteBall

/**
* White $B%o!<%k%I%b%G%k%\!<%k!#(BbodyDirection $B$H(B neckAngle $B$O>o$KL58z$G$"$k!#(B
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
* White $B%o!<%k%I%b%G%k%W%l%$%d!<!#(B
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
	
	/** $B%*%V%8%'%/%H$NBN$N8~$-$rJV$9!#(B*/
	const Angle& bodyDirection() const;
	/** $B%*%V%8%'%/%H$NF,$,!"BN$N8~$-$KBP$9$kAjBPE*$J3QEY$rJV$9!#(B*/
	const Angle& neckAngle() const;
	/** $B%*%V%8%'%/%H$N;k3P%;%s%5!<$N@dBPJ}8~$rJV$9!#(B*/
	Angle sightDirection() const;
	/** $B=jB0%A!<%`$rJV$9!#L#J}$N>l9g(B SIDE_LEFT$B!"E($N>l9g(B SIDE_RIGHT$B!#(B*/
	Side team() const;
	/** $BGXHV9f$rJV$9!#(B*/
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
* White $B%o!<%k%I%b%G%k%;%k%U!#A4$F$N%a%s%P4X?t$O!"L58zCM$rJV$5$J$$!#(B
*/
class WhiteSelf : public WhitePlayer {
protected:
	bool m_goalie;
	ViewWidth m_viewWidth;
	ViewQuality m_viewQuality;
	double m_stamina;
	double m_effort;
	Angle m_turnAngle;		// $B<!$N(B step $B$GBN$r2sE>$9$k3QEY(B
	Angle m_neckTurnAngle;	// $B<!$N(B step $B$G<s$r2sE>$9$k3QEY(B
	int m_timeLastCatch;
public:
	/** $B%4!<%k%-!<%Q!<!)(B */
	bool isGoalie() const;
	/** $B%S%e!<%b!<%I$N<A$rJV$9(B */
	ViewQuality viewQuality() const;
	/** $B%S%e!<%b!<%I$NI}$rJV$9(B */
	ViewWidth viewWidth() const;
	/** $B%S%e!<%b!<%I$NI}$rEYC10L$GJV$9(B */
	double viewWidthDegrees() const;
	/** $B%9%?%_%J$rJV$9(B */
	double stamina() const;
	/** $B%(%U%)!<%H$rJV$9(B */
	double effort() const;
	/** $B:G8e$K(B catch $B$r;n$_$?;~9o$rJV$9(B */
	double timeLastCatch() const;
	
	void initialize(int uniformNumber, bool goalie, int time,
		const Vector& position = Vector(0, 0),	// $BA4$F>o;~(B valid$B!#(B
		const Vector& velocity = Vector(0, 0),
		const Vector& acceleration = Vector(0, 0),
		Angle bodyDirection = Angle(0),
		Angle neckAngle = Angle(0));
	void step();
	
	void update(const PlayerSenseBody& sbi);
	void updateSightDirection(Angle direction);		// neckAngle $B$rJQ2=$5$;$:$K(B bodyDirection $B$r99?7$9$k(B
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
* $B%o!<%k%I%b%G%k!#$J$*!"%o!<%k%I%b%G%k$OJ#?tMQ0U$5$l$k2DG=@-$,$"$k$?$a!"?'$NL>(B
* $BA0$r7AMF;l$H$7$F;HMQ$9$k$3$H$G6hJL$9$k!#(B
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
