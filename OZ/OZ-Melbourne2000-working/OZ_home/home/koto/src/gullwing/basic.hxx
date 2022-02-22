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

/** 基本的なバッファサイズ　*/
const int BUFFER_SIZE = 8192;

/** １チームあたり最大のプレイヤー数。*/
const int MAX_PLAYER = 11;
/** デフォルトのゴールの幅。*/
const double DEFAULT_GOAL_WIDTH = 7.32;

enum Side_t {
	/** 左サイドを表す値で -1。*/
	SIDE_LEFT = -1,
		/** 右サイドを表す値で +1。*/
		SIDE_RIGHT = +1,
};
typedef EnumValue<Side_t, 0> Side;

enum Edge {
	/** 上の辺を表す値で -2。*/
	EDGE_TOP = -2,
		/** 左の辺を表す値で -1。*/
		EDGE_LEFT = -1,
		/** 右の辺を表す値で +1。*/
		EDGE_RIGHT = +1,
		/** 下の辺を表す値で +2。*/
		EDGE_BOTTOM = +2,
};

/**
* プレイモードを表す値で、0 以上 PLAYMODE_MAZ 未満。逆にこの範囲の値は        
* PlayMode のメンバ。また、PLAYMODE_LR_MAX 以下の PlayMode値 pm に対して、pm^1
* は、pm の l と r を交換した値を表す。逆に PLAYMODE_LR_MAX より大きい値は、l 
* と r の区別はない。
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
		PLAYMODE_GOALIE_CATCH_L,	// このライブラリはプレイモードとして扱う
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
	/** playMode を表す文字列を返す。*/
	const char* toString(PlayMode playMode);
	/**
	* プレイモードを表す文字列から、対応する PlayMode 値を返す。ただし、
	* teamSide == SIDE_RIGHT の場合、左右の区別を持つプレイモードでは、左右を 
    * 逆転したものを返す。文字列がプレイモードとして無効の場合、負の値を返す。
	* @param	string
	*				プレイモードを表す文字列。
	* @param	teamSide
	*				サイド。
	*/
	int toPlayMode(const char* string, Side teamSide);
	/**
	* 審判が prev から next へプレイモードが変わったと言ったとき、
	* このライブラリでは変わったと見なすか(true)どうかを返す。
	*/
	bool isPlayModeChanged(PlayMode prev, PlayMode next);
	/**
	* 引数のプレイモードにおいて、審判の時計が進行するか？
	*/
	bool isRefreeTimerProceeding(PlayMode playMode);
};
extern PlayModeHelper playModeHelper;

/**
* 審判からのメッセージを表す値で、PLAYMODE_MAZ 以上 REFREEMESSAGE_MAZ 未満。こ
* の範囲の値が RefreeMessage のメンバであることは保証されない。
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
	* 審判から送信された文字列から、対応する PlayMode 値、あるいは            
    * RefreeMessage 値を返す。ただし、teamSide == SIDE_RIGHT の場合、左右の区 
    * 別を持つ値では、左右を逆転したものを返す。文字列が審判のメッセージとして
    * 無効の場合、REFREEMESSAGE_COACH を返す（コーチによって送信されたメッセー
    * ジである）。
	* @param	string
	*				審判からのメッセージを表す文字列。
	* @param	teamSide
	*				サイド。
	*/
	int toRefreeMessage(const char* string, Side teamSide);
};
extern RefreeMessageHelper refreeMessageHelper;


/** 視覚センサーの幅を表す値 */
enum ViewWidth {
	VIEW_WIDTH_NARROW,
		VIEW_WIDTH_NORMAL,
		VIEW_WIDTH_WIDE,
};
/** ViewWidth値を表す文字列を得る。*/
inline const char* viewWidthToString(ViewWidth width)
{
	const char* strings[] = { "narrow", "normal", "wide" };
	return strings[width];
}
/** ViewWidth値のモードにおける視覚の幅を、度単位で返す。*/
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

/** 視覚センサーの質を表す値 */
enum ViewQuality {
	VIEW_QUALITY_LOW,
		VIEW_QUALITY_HIGH,
};
/** ViewQuality値を表す文字列を得る。*/
inline const char* viewQualityToString(ViewQuality quality)
{
	const char* strings[] = { "low", "high" };
	return strings[quality];
}

/** Soccer Server から送信されたメッセージの種類を表す値 */
enum InformationType {
	INFORMATION_SEE,
		INFORMATION_HEAR,
		INFORMATION_SENSE_BODY,
		INFORMATION_ERROR,
};

/** オブジェクトの種類を表す値 */
enum ObjectType {
	/** ボールを表す値で、正の値 */
	TYPE_BALL = 1,
		/** プレイヤーを表す値で、正の値 */
		TYPE_PLAYER,
		/** フラグとゴールを表す値で、正の値 */
		TYPE_FLAG,
		/** ラインを表す値で、正の値 */
		TYPE_LINE,
};
/** ObjectType_t の範囲外の値 */
const int TYPE_INVALID = 0;
//typedef EnumValue<ObjectType_t, TYPE_INVALID> ObjectType;

/**
* フラグ／ゴールを表す値で、0 以上 FLAG_MAZ 未満。逆にこの範囲の値は Flag_t の
* メンバ。
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
	/** 引数のゴール幅で初期化する。*/
	void initialize(double goalWidth = server().goal_width());
	/**
	* フラグ／ゴールを表す文字列から、対応する Flag 値を返す。ただし、teamSide
    * == SIDE_RIGHT の場合、原点に関して対称な位置にあるフラグ／ゴールを返す。
	* @param	string
	*				フラグ／ゴールを表す文字列（例えば "t l 50"）。
	* @param	teamSide
	*				サイド。
	*/
	Flag nameToFlag(const char* name, Side teamSide);
	/** フラグ／ゴール flag の位置する座標を返す。*/
	const Vector& flagToPosition(int flag);
};
/** initialize で初期化してから使用すること */
extern FlagHelper flagHelper;

/** hear 情報において、メッセージの送信元を表す値 */
enum SenderType {
	/** 審判からのメッセージ */
	SENDER_REFEREE,
		/** 自分自身からのメッセージ */
		SENDER_SELF,
	/** コーチからのメッセージ */
	SENDER_COACH,
		/** 他のプレイヤーからのメッセージ */
		SENDER_DIRECTION,
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "basic.inl"
#endif

#endif // } !defined(BASIC_HXX__INCLUDED)
