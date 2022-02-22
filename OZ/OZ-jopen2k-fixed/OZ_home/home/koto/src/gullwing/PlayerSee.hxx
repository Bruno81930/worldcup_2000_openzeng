// PlayerSee.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERSEE_HXX__INCLUDED) // {
#define PLAYERSEE_HXX__INCLUDED

#include "basic.hxx"
#if LEARNING
#include "CoachSee.hxx"
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayerSee

class PlayerSeeObject
{
#if LEARNING
public:
	// inspiration $B$GCN$C$?@5$7$$%A!<%`$HGXHV9f(B
	Side m_inspirationTeam;
	int m_inspirationUniformNumber;
#endif
private:
	Integer m_type;
	Side m_team;
	Integer m_id;
	Double m_distance;
	Angle m_direction;
	Double m_distanceChange;
	Double m_directionChange;
	Angle m_bodyDirection;
	Angle m_faceDirection;
public:
	/**
	* $B%*%V%8%'%/%H$N%?%$%W$rJV$9!#<!$N$$$:$l$+!#(B
	*		TYPE_BALL				$B%\!<%k(B
	*		TYPE_PLAYER				$B%W%l%$%d!<(B
	*		TYPE_FLAG				$B%U%i%0$+%4!<%k(B
	*		TYPE_LINE				$B%i%$%s(B
	*		$B>e$NCM$K(B-1$B$r3]$1$?CM(B	$B>e$N%7%g!<%H%G%#%9%?%s%9%;%s%5!<$GF@$i$l$?J*(B
	*/
	Integer type() const;
	/**
	* type() == TYPE_PLAYER $B$N>l9g!"%W%l%$%d!<$,L#J}$J$i$P(B SIDE_LEFT$B!"E($J$i$P(B
    * SIDE_RIGHT$B!#(B
	*/
	Side team() const;
	/**
	* type() == TYPE_PLAYER $B$N>l9g!"%W%l%$%d!<$NGXHV9f!J#0!A#1#0!K!#(B
	*/
	Integer uniformNumber() const;
	/**
	* type() == TYPE_FLAG $B$N>l9g!"%U%i%0$N<oN`!#$?$@$7!"%W%l%$%d!<$N%A!<%`$,1&(B
	* $B%5%$%I$N>l9g!"%T%C%A$NCf?4$K4X$7$FE@BP>N$J0LCV$K$"$k%U%i%0$N<oN`!#(B
	*/
	Flag flagId() const;
	/**
	* type() == TYPE_LINE $B$N>l9g!"%i%$%s$N<oN`!#$?$@$7!"%W%l%$%d!<$N%A!<%`$,1&(B
    * $B%5%$%I$N>l9g!"%T%C%A$NCf?4$K4X$7$FE@BP>N$J0LCV$K$"$k%i%$%s$N<oN`!#(B
	*/
	Edge edge() const;
	Double distance() const;
	Angle direction() const;
	Double distanceChange() const;
	Double directionChange() const;
	Angle bodyDirection() const;
	Angle faceDirection() const;
	
	/**
	* Soccer Server $B$+$iF@$?%a%C%;!<%8$+$i!"%*%V%8%'%/%H$r=i4|2=$9$k!#(B
	* @param	cursor
	*		[in]	see $B%a%C%;!<%8Cf$N!"(BPlayerSeeObject $BA4BN$r$/$/$k!J3+$-!K3g8L$X$N(B
	*				$B%]%$%s%?!#(B
	*		[out]	$BBP1~$9$kJD$83g8L$N1&$N%9%Z!<%9$N1&$NJ8;z$X$N%]%$%s%?!#(B
	* @param	teamName
	*		$B%A!<%`L>!#(B
	* @param	teamSide
	*		$B%A!<%`$N%5%$%I!#(B
	*/
	void initialize(const char* &cursor, const char* teamName, Side teamSide);
};

class PlayerSee
{
#if LEARNING
public:
	bool m_inspiration;	// inspiration $B$rF@$?$+!)(B
	CoachSee m_inspirationSee;
#endif
private:
	int m_refereeTimer;
	PlayerSeeObject* m_pBall;
	PlayerSeeObject* m_players[MAX_PLAYER*2 + 1];
	PlayerSeeObject* m_flags[FLAG_MAZ + 1];
	PlayerSeeObject* m_lines[4 + 1];
	PlayerSeeObject m_seeObjects[1 + MAX_PLAYER*2 + FLAG_MAZ + 4];
public:
	/**
	* $B?3H=$N;~7W$N;X$7$F$$$k;~9o!J(BSoccer Monitor $B$KI=<($5$l$F$$$k$N$HF1$8$b(B   
    * $B$N!K$rJV$9!#(B
	*/
	int refereeTimer() const;
	/**
	* $B;k3P>pJs$K%\!<%k$,4^$^$l$F$$$k>l9g!"$=$N>pJs$X$N%]%$%s%?!#4^$^$l$F$$$J$$(B
    * $B>l9g(B 0$B!#(B
	*/
	const PlayerSeeObject* ball() const;
	/**
	* $B;k3P>pJs$K4^$^$l$k%W%l%$%d!<$N%j%9%H$X$N%]%$%s%?!#%j%9%H$NKvHx$O(B 0 $B$G$"(B 
    * $B$k!#(B
	*/
	const PlayerSeeObject* const* players() const;
	/**
	* $B;k3P>pJs$K4^$^$l$k%U%i%0$H%4!<%k$N%j%9%H$X$N%]%$%s%?!#%j%9%H$NKvHx$O(B 0  
    * $B$G$"$k!#(B
	*/
	const PlayerSeeObject* const* flags() const;
	/**
	* $B;k3P>pJs$K4^$^$l$k%i%$%s$N%j%9%H$X$N%]%$%s%?!#%j%9%H$NKvHx$O(B 0 $B$G$"$k!#(B
	*/
	const PlayerSeeObject* const* lines() const;
	
	PlayerSee();
	PlayerSee(const PlayerSee&);
	void initialize();
	
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
#  include "PlayerSee.inl"
#endif

#endif // } !defined(PLAYERSEE_HXX__INCLUDED)
