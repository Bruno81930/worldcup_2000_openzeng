// PlayerSenseBody.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERSENSEBODY_HXX__INCLUDED) // {
#define PLAYERSENSEBODY_HXX__INCLUDED

#include "basic.hxx"
#if LEARNING
#include "CoachSee.hxx"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerSenseBody

class PlayerSenseBody
{
#if LEARNING
public:
	bool m_inspiration;	// inspiration $B$rF@$?$+!)(B
	CoachSee m_inspirationSee;
#endif
private:
	int m_refereeTimer;
	ViewQuality m_viewQuality;
	ViewWidth m_viewWidth;
	double m_stamina;
	double m_effort;
	double m_speed;
	Angle m_neckAngle;
	int m_kickCount;
	int m_dashCount;
	int m_turnCount;
	int m_sayCount;
	int m_turnNeckCount;
public:
	/**
	* $B?3H=$N;~7W$N;X$7$F$$$k;~9o!J(BSoccer Monitor $B$KI=<($5$l$F$$$k$N$HF1$8$b(B   
    * $B$N!K$rJV$9!#(B
	*/
	int refereeTimer() const;
	ViewQuality viewQuality() const;
	ViewWidth viewWidth() const;
	double stamina() const;
	double effort() const;
	double speed() const;
	/**
	* $BLa$jCM$O>o$K(B valid() $B$G$"$k!#(B
	*/
	Angle neckAngle() const;
	int kickCount() const;
	int dashCount() const;
	int turnCount() const;
	int sayCount() const;
	int turnNeckCount() const;
	
	PlayerSenseBody();
	void initialize();
	/**
	* Soccer Server $B$+$iF@$?%a%C%;!<%8$+$i!"%*%V%8%'%/%H$r=i4|2=$9$k!#(B
	* @param	body
	*		Soccer Server $B$+$iF@$?(B sense_body $B%a%C%;!<%8$N!"(Btime $B%Q%i%a%?$N@h(B 
	*		$BF,$NJ8;z$X$N%]%$%s%?!#(B
	* @param	teamName
	*		$B%A!<%`L>!#(B
	* @param	teamSide
	*		$B%A!<%`$N%5%$%I!#(B
	*/
	void initialize(const char* body, const char* teamName, Side teamSide);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerSenseBody.inl"
#endif

#endif // } !defined(PLAYERSENSEBODY_HXX__INCLUDED)
