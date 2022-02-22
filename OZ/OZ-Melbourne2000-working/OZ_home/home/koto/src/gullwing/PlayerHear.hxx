// PlayerHear.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(PLAYERHEAR_HXX__INCLUDED) // {
#define PLAYERHEAR_HXX__INCLUDED

#include "basic.hxx"

/////////////////////////////////////////////////////////////////////////////
// PlayerHear

class PlayerHear
{
private:
	int m_refereeTimer;
	SenderType m_senderType;
	Angle m_direction;
	char* m_message;
	int m_refereeMessage;
	int m_extra;
public:
	/**
	* $B?3H=$N;~7W$N;X$7$F$$$k;~9o!J(BSoccer Monitor $B$KI=<($5$l$F$$$k$N$HF1$8$b(B   
    * $B$N!K$rJV$9!#(B
	*/
	int refereeTimer() const;
	/**
	* $BAw?.<T$N<oN`$rJV$9!#<!$N$$$:$l$+!#(B
	*		SENDER_REFEREE		$B%l%U%j!<$+$i$N%a%C%;!<%8(B
	*		SENDER_SELF			$B<+J,<+?H$+$i$N%a%C%;!<%8(B
	*		SENDER_DIRECTION	$BB>$N%W%l%$%d!<$+$i$N%a%C%;!<%8(B
	*/
	SenderType senderType() const;
	/**
	* senderType() == SENDER_DIRECTION $B$N>l9g!"Aw?.<T$NJ}8~!#(B
	*/
	Angle direction() const;
	/**
	* senderType() == SENDER_REFEREE $B$N>l9g!"?3H=$N%a%C%;!<%8!#<!$N$$$:$l$+!#(B
	*		PLAYMODE_*			$B%W%l%$%b!<%IJQ99$N%a%C%;!<%8!#$?$@$7!"%W%l%$%d(B
	*							$B!<$N%A!<%`$,1&%5%$%I$N>l9g!":81&$N6hJL$r;}$D%W(B
	*							$B%l%$%b!<%I$O!":81&$r5UE>$7$?$b$N!#(B
	*		REFREEMESSAGE_*		$B$=$NB>$N%a%C%;!<%8!#$?$@$7!"%W%l%$%d!<$N%A!<%`(B
	*							$B$,1&%5%$%I$N>l9g!":81&$N6hJL$r;}$D%W%l%$%b!<%I(B
	*							$B$O!":81&$r5UE>$7$?$b$N!#(B
	*		REFREEMESSAGE_COACH	$B%3!<%A$+$i$N%a%C%;!<%8!)(B
	*/
	int refereeMessage() const;
	/**
	* refereeMessage() == PLAYMODE_GOAL_L || refereeMessage() == PLAYMODE_GOAL
    * _R $B$N$H$-!"2?E@L\$,F~$C$?$+$rJV$9!#(B
	*/
	int score() const;
	/**
	* $B<u?.$7$?%a%C%;!<%8$rJV$9!#(B
	*/
	const char* message() const;
	
	~PlayerHear();
	PlayerHear();
	PlayerHear(const PlayerHear&);
	void initialize();
	/**
	* Soccer Server $B$+$iF@$?%a%C%;!<%8$+$i!"%*%V%8%'%/%H$r=i4|2=$9$k!#(B
	* @param	body
	*		Soccer Server $B$+$iF@$?(B hear $B%a%C%;!<%8$N!"(Btime $B%Q%i%a%?$N@hF,$NJ8(B 
	*		$B;z$X$N%]%$%s%?!#(B
	* @param	teamSide
	*		$B%W%l%$%d!<$N%A!<%`$N%5%$%I!#(B
	*/
	void initialize(const char* body, Side teamSide);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "PlayerHear.inl"
#endif

#endif // } !defined(PLAYERHEAR_HXX__INCLUDED)
