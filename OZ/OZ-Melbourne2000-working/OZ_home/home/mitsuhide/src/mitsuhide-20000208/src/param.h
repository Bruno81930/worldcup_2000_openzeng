#define HOST_NAME      "localhost"
#define PORT_NUMBER    6000
#define VERSION_NUMBER 5.25
#define TEAM_NAME      "YowAI"
#define GOALIE_FLAG    false

#define MESSAGE_LENGTH 4096

// 後で整理する

typedef enum {
  PM_Null,
  PM_BeforeKickOff,
  PM_TimeOver,
  PM_PlayOn,
  PM_KickOff_Left,
  PM_KickOff_Right,
  PM_KickIn_Left,
  PM_KickIn_Right,
  PM_FreeKick_Left,
  PM_FreeKick_Right,
  PM_CornerKick_Left,
  PM_CornerKick_Right,
  PM_GoalKick_Left,
  PM_GoalKick_Right,
  PM_AfterGoal_Left,
  PM_AfterGoal_Right,
  PM_Drop_Ball,
  PM_OffSide_Left,
  PM_OffSide_Right,
  PM_Error,
  PM_MAX
} PlayMode;

#define PLAYMODE_TABLE	{\
  {"half_time", PM_BeforeKickOff},\
  {"time_extended", PM_BeforeKickOff},\
  {"before_kick_off", PM_BeforeKickOff},\
  {"time_up_without_a_team", PM_TimeOver},\
  {"time_up", PM_TimeOver},\
  {"time_over", PM_TimeOver},\
  {"play_on", PM_PlayOn},\
  {"kick_off_l", PM_KickOff_Left},\
  {"kick_off_r", PM_KickOff_Right},\
  {"kick_in_l", PM_KickIn_Left},\
  {"kick_in_r", PM_KickIn_Right},\
  {"foul_l", PM_FreeKick_Right},\
  {"foul_r", PM_FreeKick_Left},\
  {"goalie_catch_ball_l", PM_FreeKick_Left},\
  {"goalie_catch_ball_r", PM_FreeKick_Right},\
  {"free_kick_l", PM_FreeKick_Left},\
  {"free_kick_r", PM_FreeKick_Right},\
  {"corner_kick_l", PM_CornerKick_Left},\
  {"corner_kick_r", PM_CornerKick_Right},\
  {"goal_kick_l", PM_GoalKick_Left},\
  {"goal_kick_r", PM_GoalKick_Right},\
  {"goal_l", PM_AfterGoal_Left},\
  {"goal_r", PM_AfterGoal_Right},\
  {"drop_ball", PM_Drop_Ball},\
  {"offside_l", PM_OffSide_Left},\
  {"offside_r", PM_OffSide_Right},\
  {"Error", PM_Error},\
  {NULL}}

typedef struct {
  char* name ;
  PlayMode id;
} PlayModeEntry;

extern const PlayModeEntry pm_entry[];

typedef enum {
  VW_NARROW,
  VW_NORMAL,
  VW_WIDE
} ViewWidth;

typedef enum {
  VQ_LOW,
  VQ_HIGH
} ViewQuality;

typedef enum {
  VT_40,  
  VT_80,  
  VT_160, 
  VT_320  
} ViewType;

#define STAMINA_MAX 3500.0
#define EFFORT_MAX 1.0
#define RECOVER_MAX 1.0
#define MARGIN_FOR_SEND_COMMAND 50

typedef enum {
  ST_0,  
  ST_20, 
  ST_40, 
  ST_60, 
  ST_80  
} SeeType;

#define EPS 1.0e-10

#define MESSAGE_LENGTH 4096
#define MARGIN_FOR_SEND_COMMAND 50
#define STAMINA_MAX 3500.0
#define STAMINA_INC_MAX 35.0
#define EFFORT_MAX 1.0
#define EFFORT_DEC_THR 0.3
#define EFFORT_INC_THR 0.6
#define EFFORT_INC 0.01
#define EFFORT_DEC 0.005
#define EFFORT_MIN 0.6
#define RECOVER_MAX 1.0
#define RECOVER_DEC_THR 0.3
#define RECOVER_DEC 0.002
#define RECOVER_MIN 0.5
#define PLAYER_DECAY 0.4
#define MAXPOWER 100.0
#define MINPOWER -100.0
#define DASH_POWER_RATE 0.006
#define INERTIA_MOMENT 5.0
#define MAXMOMENT 180.0
#define MINMOMENT -180.0
#define MAXNECKMOMENT 180.0
#define MINNECKMOMENT -180.0
#define MAXNECKANG 90.0
#define MINNECKANG -90.0
#define PLAYER_RAND 0.1
#define PLAYER_SPEED_MAX 1.0
#define KICK_POWER_RATE 0.016
#define BALL_DECAY 0.94
#define BALL_RAND 0.05
#define BALL_SPEED_MAX 2.7
#define PLAYER_SIZE 0.3
#define BALL_SIZE 0.085
#define KICKABLE_MARGIN 0.7
#define KICKABLE (PLAYER_SIZE + BALL_SIZE + KICKABLE_MARGIN)
#define CATCHABLE_AREA_L 2.0
