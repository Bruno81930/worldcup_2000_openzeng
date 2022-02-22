// CoachSee.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(COACHSEE_HXX__INCLUDED) // {
#define COACHSEE_HXX__INCLUDED

#include "basic.hxx"

/////////////////////////////////////////////////////////////////////////////
// CoachSee

class CoachSeeObject
{
private:
	bool m_valid;
	ObjectType m_type;
	Side m_team;
	Integer m_uniformNumber;
	Vector m_position;
	Vector m_velocity;
	Angle m_bodyDirection;
	Angle m_neckAngle;
	
public:
	bool valid() const;
	ObjectType type() const;
	Side team() const;
	int uniformNumber() const;
	Vector position() const;
	Vector velocity() const;
	Angle bodyDirection() const;
	Angle neckAngle() const;

	CoachSeeObject();
	
	void initialize();
	/**
	* Soccer Server $B$+$iF@$?%a%C%;!<%8$+$i!"%*%V%8%'%/%H$r=i4|2=$9$k!#(B
	* @param	cursor
	*		[in]	see $B%a%C%;!<%8Cf$N!"(BCoachSeeObject $BA4BN$r$/$/$k!J3+$-!K3g8L$X$N(B
	*				$B%]%$%s%?!#(B
	*		[out]	$BBP1~$9$kJD$83g8L$N:8$N%9%Z!<%9$N:8$NJ8;z$X$N%]%$%s%?!#(B
	* @param	teamName
	*		$B%A!<%`L>!#(B
	* @param	teamSide
	*		$B%A!<%`$N%5%$%I!#(B
	*/
	void initialize(const char* &cursor, const char* teamName, Side teamSide);
};

class CoachSee
{
public:
	typedef std::vector<CoachSeeObject> Objects;

private:
	/*CoachSee(const CoachSee&);
	CoachSee& operator= (const CoachSee&);
	bool operator== (const CoachSee&) const;*/
	
private:
	int m_refreeTimer;
	CoachSeeObject m_goals[2];
	CoachSeeObject m_ball;
	CoachSeeObject m_players[2][MAX_PLAYER];
	
public:
	const CoachSeeObject& ball() const;
	const CoachSeeObject& player(Side team, int uniformNumber) const;
	
	CoachSee();
	
	/**
	* Soccer Server $B$+$iF@$?%a%C%;!<%8$+$i!"%*%V%8%'%/%H$r=i4|2=$9$k!#(B
	* @param	body
	*		Soccer Server $B$+$iF@$?(B see $B%a%C%;!<%8$N!"(Btime $B%Q%i%a%?$N@hF,$NJ8;z(B
	*		$B$X$N%]%$%s%?!#(B
	* @param	teamName
	*		$B%A!<%`L>!#(B
	* @param	teamSide
	*		$B%A!<%`$N%5%$%I!#(B
	*/
	void initialize(const char* body, const char* teamName, Side teamSide);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "CoachSee.inl"
#endif

#endif // } !defined(COACHSEE_HXX__INCLUDED)
