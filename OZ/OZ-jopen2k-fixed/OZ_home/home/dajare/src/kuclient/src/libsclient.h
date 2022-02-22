/*
 *	Header:
 *	File: libsclient.h
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *	Changed by Yusuke Yamauchi (1999/06/12)
 *	EndHeader:
 */

#define Bool		int
#define SEENOBJMAX	96
#define BUFSIZE		2048
#define TRUE		1
#define FALSE		0
#define DIST_ERR	-360
#define DIR_ERR		-360
#define CHNG_ERR	1024
#define UNUM_ERR	-1

#define PITCH_LENGTH	105.0
#define PITCH_WIDTH	68.0
#define PENALTY_AREA_LENGTH	16.5
#define PENALTY_AREA_WIDTH	40.32
#define GOAL_AREA_LENGTH	5.5
#define GOAL_AREA_WIDTH	18.32
#define GOAL_WIDTH	14.02

#define Abs(x)  (((x) > 0.0)?(x):(-(x)))
#define Pow(x)  ((x)*(x))
#define Rad2Deg(x)  ((double)(x) * 180.0 / M_PI)
#define Deg2Rad(x)  ((double)(x) * M_PI / 180.0)

#ifdef MEMSET
#define bzero(a, b) memset(a, 0, b)
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct socket {
        int socketfd  ;
        struct sockaddr_in serv_addr ;
} Socket ;

typedef enum _Side {
        S_UNKNOWN,
        S_LEFT,
        S_RIGHT,
} Side ;

#define PLAYMODE_STRINGS {\
        "before_kick_off",\
        "play_on",\
        "time_over",\
        "kick_off_l",\
        "kick_off_r",\
        "kick_in_l",\
        "kick_in_r",\
        "free_kick_l",\
        "free_kick_r",\
        "corner_kick_l",\
        "corner_kick_r",\
        "goal_kick_l",\
        "goal_kick_r",\
        "goal_l",\
        "goal_r",\
        NULL\
}

typedef enum _PlayMode {
        PM_BeforeKickOff,
        PM_PlayOn,
        PM_TimeOver,
        PM_KickOff_L,
        PM_KickOff_R,
        PM_KickIn_L,
        PM_KickIn_R,
        PM_FreeKick_L,
        PM_FreeKick_R,
        PM_CornerKick_L,
        PM_CornerKick_R,
        PM_GoalKick_L,
        PM_GoalKick_R,
        PM_Goal_L,
        PM_Goal_R,
        PM_Error,
} PlayMode ;

typedef struct _InitInfo {
        Side side ;
        int unum ;
        char playmodestr[32] ;
        PlayMode playmode ;
} InitInfo ;


typedef enum _ViewWidth {
        VW_Narrow,
        VW_Normal,
        VW_Wide,
} ViewWidth ;

typedef enum _ViewQuality {
        VQ_high,
        VQ_low,
} ViewQuality ;

typedef enum _ObjectType {
        OT_Unknown,
        OT_Ball,
        OT_Player,
        OT_Goal,
        OT_Flag,
        OT_Line,
} ObjectType ;

typedef enum _FlagPos {
        FP_Unknown,
        FP_T_L_50, FP_T_L_40, FP_T_L_30, FP_T_L_20,
        FP_T_L_10, FP_T_0,    FP_T_R_10,
        FP_T_R_20, FP_T_R_30, FP_T_R_40, FP_T_R_50,

        FP_L_T,    FP_C_T,    FP_R_T,
        FP_L_T_30, FP_R_T_30,
        FP_L_T_20, FP_R_T_20,
        FP_P_L_T,  FP_P_R_T,
        FP_L_T_10, FP_R_T_10,
        FP_G_L_T,  FP_G_R_T,

        FP_L_0,    FP_P_L_C,  FP_C,
        FP_P_R_C,  FP_R_0,

        FP_G_L_B,  FP_G_R_B,
        FP_L_B_10, FP_R_B_10,
        FP_P_L_B,  FP_P_R_B,
        FP_L_B_20, FP_R_B_20,
        FP_L_B_30, FP_R_B_30,
        FP_L_B,    FP_C_B,    FP_R_B,

        FP_B_L_50, FP_B_L_40, FP_B_L_30, FP_B_L_20,
        FP_B_L_10, FP_B_0,    FP_B_R_10,
        FP_B_R_20, FP_B_R_30, FP_B_R_40, FP_B_R_50,
} FlagPos ;

typedef enum _LinePos {
        LP_Unknown,
        LP_L,
        LP_T,
        LP_B,
        LP_R, 
} LinePos ;

typedef enum _WhichTeam {
        WT_Opponent = -1,
        WT_Unknown = 0,
        WT_Our = 1,
} WhichTeam ;

typedef enum _PlayStile {
        PS_Field,
        PS_Goalie,
} PlayStile ;

typedef struct _Ppos {
        WhichTeam team ;
        int unum ;
} PlayerInfo ;

typedef struct _SeenObj {
        ObjectType type ;
        Bool inViewConeP ; /* True if the object is in the view cone */
        union {
                PlayerInfo player ;
                Side side ;
                FlagPos fpos ;
                LinePos lpos ;
        } id ;
        double dist ;
        double dir ;
        double dist_chng ;
        double dir_chng ;
        double face_dir ;
} SeenObj ;

typedef struct _SeeInfo {
        int time ;
        int n_obj ;
        SeenObj obj[SEENOBJMAX] ;
} SeeInfo ;

typedef enum _SenderType {
        ST_Self,
        ST_Referee,
        ST_Player,
} SenderType ;

typedef struct _HearInfo {
        int time ;
        SenderType sender ;
        double direction ;
        char message[BUFSIZE] ;
} HearInfo ;

typedef struct _BodyInfo {
        int time ;
        ViewWidth viewwidth ;
        ViewQuality viewquality ;
        double stamina ;
        double effort ;
        double speed ;
        int kick ;
        int dash ;
        int turn ;
        int say ;
        int turn_neck ;
        double head_angle ;
} BodyInfo ;

typedef enum _SensorInfoType {
        SIT_See,
        SIT_Hear,
        SIT_Body,
} SensorInfoType ;

typedef struct _SensorInfo {
        SensorInfoType type ;
        union {
                SeeInfo see ;
                HearInfo hear ;
                BodyInfo body ;
        } info ;
} SensorInfo ;

typedef struct _XYPos {
        double x ;
        double y ;
} Pos ;

typedef struct _PosState {
        Pos pos ;
        double dir ;
} PosState ;

