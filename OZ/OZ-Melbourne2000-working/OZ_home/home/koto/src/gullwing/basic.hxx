// basic.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(BASIC_HXX__INCLUDED) // {
#define BASIC_HXX__INCLUDED

#include "Server.hxx"
#include "EnumValue.hxx"
#include "Vector.hxx"

/////////////////////////////////////////////////////////////////////////////
// basic

/** $B4pK\E*$J%P%C%U%!%5%$%:!!(B*/
const int BUFFER_SIZE = 8192;

/** $B#1%A!<%`$"$?$j:GBg$N%W%l%$%d!<?t!#(B*/
const int MAX_PLAYER = 11;
/** $B%G%U%)%k%H$N%4!<%k$NI}!#(B*/
const double DEFAULT_GOAL_WIDTH = 7.32;

enum Side_t {
	/** $B:8%5%$%I$rI=$9CM$G(B -1$B!#(B*/
	SIDE_LEFT = -1,
		/** $B1&%5%$%I$rI=$9CM$G(B +1$B!#(B*/
		SIDE_RIGHT = +1,
};
typedef EnumValue<Side_t, 0> Side;

enum Edge {
	/** $B>e$NJU$rI=$9CM$G(B -2$B!#(B*/
	EDGE_TOP = -2,
		/** $B:8$NJU$rI=$9CM$G(B -1$B!#(B*/
		EDGE_LEFT = -1,
		/** $B1&$NJU$rI=$9CM$G(B +1$B!#(B*/
		EDGE_RIGHT = +1,
		/** $B2<$NJU$rI=$9CM$G(B +2$B!#(B*/
		EDGE_BOTTOM = +2,
};

/**
* $B%W%l%$%b!<%I$rI=$9CM$G!"(B0 $B0J>e(B PLAYMODE_MAZ $BL$K~!#5U$K$3$NHO0O$NCM$O(B        
* PlayMode $B$N%a%s%P!#$^$?!"(BPLAYMODE_LR_MAX $B0J2<$N(B PlayMode$BCM(B pm $B$KBP$7$F!"(Bpm^1
* $B$O!"(Bpm $B$N(B l $B$H(B r $B$r8r49$7$?CM$rI=$9!#5U$K(B PLAYMODE_LR_MAX $B$h$jBg$-$$CM$O!"(Bl 
* $B$H(B r $B$N6hJL$O$J$$!#(B
*/
enum PlayMode {
	PLAYMODE_KICK_OFF_L,
		PLAYMODE_KICK_OFF_R,
		PLAYMODE_KICK_IN_L,
		PLAYMODE_KICK_IN_R,
		PLAYMODE_FREE_KICK_L,
		PLAYMODE_FREE_KICK_R,
		PLAYMODE_CORNER_KICK_L,
		PLAYMODE_CORNER_KICK_R,
		PLAYMODE_GOAL_KICK_L,
		PLAYMODE_GOAL_KICK_R,
		PLAYMODE_GOAL_L,
		PLAYMODE_GOAL_R,
		PLAYMODE_OFFSIDE_L,
		PLAYMODE_OFFSIDE_R,
		PLAYMODE_GOALIE_CATCH_L,	// $B$3$N%i%$%V%i%j$O%W%l%$%b!<%I$H$7$F07$&(B
		PLAYMODE_GOALIE_CATCH_R,
		PLAYMODE_BEFORE_KICK_OFF,
		PLAYMODE_TIME_OVER,
		PLAYMODE_PLAY_ON,
		PLAYMODE_DROP_BALL,
		
};
const int PLAYMODE_MAZ = PLAYMODE_DROP_BALL + 1;
const int PLAYMODE_LR_MAX = PLAYMODE_GOALIE_CATCH_R;

class PlayModeHelper {
private:
	PlayModeHelper(const PlayModeHelper&);
private:
	static const char* const m_playmodeStrings[PLAYMODE_MAZ];
public:
	~PlayModeHelper();
	PlayModeHelper();
	/** playMode $B$rI=$9J8;zNs$rJV$9!#(B*/
	const char* toString(PlayMode playMode);
	/**
	* $B%W%l%$%b!<%I$rI=$9J8;zNs$+$i!"BP1~$9$k(B PlayMode $BCM$rJV$9!#$?$@$7!"(B
	* teamSide == SIDE_RIGHT $B$N>l9g!":81&$N6hJL$r;}$D%W%l%$%b!<%I$G$O!":81&$r(B 
    * $B5UE>$7$?$b$N$rJV$9!#J8;zNs$,%W%l%$%b!<%I$H$7$FL58z$N>l9g!"Ii$NCM$rJV$9!#(B
	* @param	string
	*				$B%W%l%$%b!<%I$rI=$9J8;zNs!#(B
	* @param	teamSide
	*				$B%5%$%I!#(B
	*/
	int toPlayMode(const char* string, Side teamSide);
	/**
	* $B?3H=$,(B prev $B$+$i(B next $B$X%W%l%$%b!<%I$,JQ$o$C$?$H8@$C$?$H$-!"(B
	* $B$3$N%i%$%V%i%j$G$OJQ$o$C$?$H8+$J$9$+(B(true)$B$I$&$+$rJV$9!#(B
	*/
	bool isPlayModeChanged(PlayMode prev, PlayMode next);
	/**
	* $B0z?t$N%W%l%$%b!<%I$K$*$$$F!"?3H=$N;~7W$,?J9T$9$k$+!)(B
	*/
	bool isRefreeTimerProceeding(PlayMode playMode);
};
extern PlayModeHelper playModeHelper;

/**
* $B?3H=$+$i$N%a%C%;!<%8$rI=$9CM$G!"(BPLAYMODE_MAZ $B0J>e(B REFREEMESSAGE_MAZ $BL$K~!#$3(B
* $B$NHO0O$NCM$,(B RefreeMessage $B$N%a%s%P$G$"$k$3$H$OJ]>Z$5$l$J$$!#(B
*/
enum RefreeMessage {
	REFREEMESSAGE_TIME_UP_WITHOUT_A_TEAM = PLAYMODE_MAZ,
		REFREEMESSAGE_TIME_UP,
		REFREEMESSAGE_TIME_EXTENDED,
		REFREEMESSAGE_HALF_TIME,
		REFREEMESSAGE_FAUL_L = (REFREEMESSAGE_HALF_TIME & ~1) + 2,
		REFREEMESSAGE_FAUL_R,
		REFREEMESSAGE_GOALIE_CATCH_BALL_L,
		REFREEMESSAGE_GOALIE_CATCH_BALL_R,
		REFREEMESSAGE_COACH,
};
const int REFREEMESSAGE_MAZ = REFREEMESSAGE_GOALIE_CATCH_BALL_R + 1;

class RefreeMessageHelper {
	static const char* const m_refreeMessageStrings[];
public:
	/**
	* $B?3H=$+$iAw?.$5$l$?J8;zNs$+$i!"BP1~$9$k(B PlayMode $BCM!"$"$k$$$O(B            
    * RefreeMessage $BCM$rJV$9!#$?$@$7!"(BteamSide == SIDE_RIGHT $B$N>l9g!":81&$N6h(B 
    * $BJL$r;}$DCM$G$O!":81&$r5UE>$7$?$b$N$rJV$9!#J8;zNs$,?3H=$N%a%C%;!<%8$H$7$F(B
    * $BL58z$N>l9g!"(BREFREEMESSAGE_COACH $B$rJV$9!J%3!<%A$K$h$C$FAw?.$5$l$?%a%C%;!<(B
    * $B%8$G$"$k!K!#(B
	* @param	string
	*				$B?3H=$+$i$N%a%C%;!<%8$rI=$9J8;zNs!#(B
	* @param	teamSide
	*				$B%5%$%I!#(B
	*/
	int toRefreeMessage(const char* string, Side teamSide);
};
extern RefreeMessageHelper refreeMessageHelper;


/** $B;k3P%;%s%5!<$NI}$rI=$9CM(B */
enum ViewWidth {
	VIEW_WIDTH_NARROW,
		VIEW_WIDTH_NORMAL,
		VIEW_WIDTH_WIDE,
};
/** ViewWidth$BCM$rI=$9J8;zNs$rF@$k!#(B*/
inline const char* viewWidthToString(ViewWidth width)
{
	const char* strings[] = { "narrow", "normal", "wide" };
	return strings[width];
}
/** ViewWidth$BCM$N%b!<%I$K$*$1$k;k3P$NI}$r!"EYC10L$GJV$9!#(B*/
inline double viewWidthToDegrees(ViewWidth viewWidth) {
	double degrees = server().visible_angle();
	switch(viewWidth) {
	default:
		ASSERT(false);
	case VIEW_WIDTH_NARROW:
		return degrees/2;
	case VIEW_WIDTH_NORMAL:
		return degrees;
	case VIEW_WIDTH_WIDE:
		;
	}
	return degrees*2;
}

/** $B;k3P%;%s%5!<$N<A$rI=$9CM(B */
enum ViewQuality {
	VIEW_QUALITY_LOW,
		VIEW_QUALITY_HIGH,
};
/** ViewQuality$BCM$rI=$9J8;zNs$rF@$k!#(B*/
inline const char* viewQualityToString(ViewQuality quality)
{
	const char* strings[] = { "low", "high" };
	return strings[quality];
}

/** Soccer Server $B$+$iAw?.$5$l$?%a%C%;!<%8$N<oN`$rI=$9CM(B */
enum InformationType {
	INFORMATION_SEE,
		INFORMATION_HEAR,
		INFORMATION_SENSE_BODY,
		INFORMATION_ERROR,
};

/** $B%*%V%8%'%/%H$N<oN`$rI=$9CM(B */
enum ObjectType {
	/** $B%\!<%k$rI=$9CM$G!"@5$NCM(B */
	TYPE_BALL = 1,
		/** $B%W%l%$%d!<$rI=$9CM$G!"@5$NCM(B */
		TYPE_PLAYER,
		/** $B%U%i%0$H%4!<%k$rI=$9CM$G!"@5$NCM(B */
		TYPE_FLAG,
		/** $B%i%$%s$rI=$9CM$G!"@5$NCM(B */
		TYPE_LINE,
};
/** ObjectType_t $B$NHO0O30$NCM(B */
const int TYPE_INVALID = 0;
//typedef EnumValue<ObjectType_t, TYPE_INVALID> ObjectType;

/**
* $B%U%i%0!?%4!<%k$rI=$9CM$G!"(B0 $B0J>e(B FLAG_MAZ $BL$K~!#5U$K$3$NHO0O$NCM$O(B Flag_t $B$N(B
* $B%a%s%P!#(B
*/
enum Flag_t {
	FLAG_LT=0, FLAG_CT, FLAG_RT,
		GOAL_L,    FLAG_C,  GOAL_R,
		FLAG_LB,   FLAG_CB, FLAG_RB,
		
		FLAG_PLT, FLAG_PRT,
		FLAG_PLC, FLAG_PRC,
		FLAG_PLB, FLAG_PRB,
		
		FLAG_GLT, FLAG_GRT,
		FLAG_GLB, FLAG_GRB,
		
		FLAG_TL50, FLAG_TL40, FLAG_TL30, FLAG_TL20, FLAG_TL10, FLAG_T0, FLAG_TR10, FLAG_TR20, FLAG_TR30, FLAG_TR40, FLAG_TR50,
		FLAG_BL50, FLAG_BL40, FLAG_BL30, FLAG_BL20, FLAG_BL10, FLAG_B0, FLAG_BR10, FLAG_BR20, FLAG_BR30, FLAG_BR40, FLAG_BR50,
		
		FLAG_LT30, FLAG_LT20, FLAG_LT10, FLAG_L0, FLAG_LB10, FLAG_LB20, FLAG_LB30,
		FLAG_RT30, FLAG_RT20, FLAG_RT10, FLAG_R0, FLAG_RB10, FLAG_RB20, FLAG_RB30,
};
const int FLAG_MAZ = FLAG_RB30 + 1;
typedef EnumValue<Flag_t, -1> Flag;

class FlagHelper {
private:
	FlagHelper(const FlagHelper&);
private:
	bool m_initialized;
	static const Flag m_flagToSymmetric[FLAG_MAZ];
	static const char* const m_flagNames[FLAG_MAZ];
	Vector m_flagToPosition[FLAG_MAZ];
public:
	FlagHelper();
	/** $B0z?t$N%4!<%kI}$G=i4|2=$9$k!#(B*/
	void initialize(double goalWidth = server().goal_width());
	/**
	* $B%U%i%0!?%4!<%k$rI=$9J8;zNs$+$i!"BP1~$9$k(B Flag $BCM$rJV$9!#$?$@$7!"(BteamSide
    * == SIDE_RIGHT $B$N>l9g!"86E@$K4X$7$FBP>N$J0LCV$K$"$k%U%i%0!?%4!<%k$rJV$9!#(B
	* @param	string
	*				$B%U%i%0!?%4!<%k$rI=$9J8;zNs!JNc$($P(B "t l 50"$B!K!#(B
	* @param	teamSide
	*				$B%5%$%I!#(B
	*/
	Flag nameToFlag(const char* name, Side teamSide);
	/** $B%U%i%0!?%4!<%k(B flag $B$N0LCV$9$k:BI8$rJV$9!#(B*/
	const Vector& flagToPosition(int flag);
};
/** initialize $B$G=i4|2=$7$F$+$i;HMQ$9$k$3$H(B */
extern FlagHelper flagHelper;

/** hear $B>pJs$K$*$$$F!"%a%C%;!<%8$NAw?.85$rI=$9CM(B */
enum SenderType {
	/** $B?3H=$+$i$N%a%C%;!<%8(B */
	SENDER_REFEREE,
		/** $B<+J,<+?H$+$i$N%a%C%;!<%8(B */
		SENDER_SELF,
	/** $B%3!<%A$+$i$N%a%C%;!<%8(B */
	SENDER_COACH,
		/** $BB>$N%W%l%$%d!<$+$i$N%a%C%;!<%8(B */
		SENDER_DIRECTION,
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "basic.inl"
#endif

#endif // } !defined(BASIC_HXX__INCLUDED)
