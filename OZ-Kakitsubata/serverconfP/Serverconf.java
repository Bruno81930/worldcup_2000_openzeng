// Serverconf.java

package serverconfP;

/********************************************************/
/*         Serverconf サッカーサーバーの設定値          */
/********************************************************/

public interface Serverconf {

  int ON = 1;
  int OFF = 0;

/*  Configurations for soccerserver  */
/*  Lines that start '#' are comment lines. */
  double GOAL_WIDTH = 14.02;

/*  size, decay, random parameter, weight, maximum speed of players */
/*  (default 1.0, 0.5, 0.1, 60.0, 32.0) */
  double PLAYER_SIZE = 0.3;
  double PLAYER_DECAY = 0.4;
  double PLAYER_RAND = 0.1;
  double PLAYER_WEIGHT = 60.0;
  double PLAYER_SPEED_MAX = 1.0;
  // neighborhood 追加  マニュアルでは3.0
  double NEIGHBORHOOD = 3.0;

/*  maximum and recovery step of player's stamina (default 2500.0, 50.0) */
  double STAMINA_MAX = 3500.0;
  double STAMINA_INC_MAX = 35.0;

/*  decriment threshold ,decriment step and minimum of player's recovery */
/*  (default 0.3, 0.05, 0.1) */
  double RECOVER_DEC_THR = 0.3;
  double RECOVER_DEC = 0.002;
  double RECOVER_MIN = 0.5;

/*  decriment threshold, decriment step, incriment threshold, incriment step */
/*  and minimum of player's effort (default 0.4, 0.05, 0.9, 0.05, 0.1)  */
  double EFFORT_DEC_THR	= 0.3;
  double EFFORT_DEC = 0.005;
  double EFFORT_INC_THR	= 0.6;
  double EFFORT_INC = 0.01;
  double EFFORT_MIN = 0.6;

/*  maximum, recovery step and decay of player's hear capacity (default 2, 1, 1) */
  double HEAR_MAX = 2;
  double HEAR_INC = 1;
  double HEAR_DECAY = 2;

/*  inertia moment of player (default 5.0) */
  double INERTIA_MOMENT	= 5.0;

/*  interval of sense_body (default 100) */
  double SENSE_BODY_STEP = 100;

/*  goalie catchable area length, width (default 2.0, 1.0) */
  double CATCHABLE_AREA_L = 2.0;
  double CATCHABLE_AREA_W = 1.0;

/*  goalie catchbale probability (default 1.0) */
  double CATCH_PROBABILITY = 1.0;

/*  goalie catch ban cycle (default 5) */
  double CATCH_BAN_CYCLE = 5;

/*  size, decay, random parameter, weight and maximum speed of a ball */
/*  (default 0.15, 0.8, 0.2, 0.2, 32.0) */
  double BALL_SIZE  = 0.085;
  double BALL_DECAY = 0.94;
  double BALL_RAND = 0.05;
  double BALL_WEIGHT = 0.2;
  double BALL_SPEED_MAX = 2.7;

/*  force, direction and random parameter of wind (default 10.0, 0.0, 0.3) */
  double WIND_FORCE = 0.0;
  double WIND_DIR = 0.0;
  double WIND_RAND = 0.0;

/*  kickable margin (default 1.0) kickable_area = kickable_margin + bsize + psize */
  double KICKABLE_MARGIN = 0.7;

/*  corner kick margin (default 1.0) */
  double CKICK_MARGIN = 1.0;

/*  magnification of power in dash, kick  (default 0.1, 0.1) */
  double DASH_POWER_RATE = 0.006;
/* kick_power_rate	: 0.013 */
  double KICK_POWER_RATE = 0.016;

/*  angle of view corn [unit: degree] (default 90.0) */
  double VISIBLE_ANGLE = 90.0;

/*  audio cut off distance (default 50.0) */
  double AUDIO_CUT_DIST = 50.0;

/*  quantize step of distance for move_object, landmark (default 0.1, 0.01) */
  double QUANTIZE_STEP = 0.1;
  double QUANTIZE_STEP_L = 0.01;

/*  max and min of power in dash and kick (default 100, -30) */
  double MAXPOWER = 100;
  double MINPOWER = -100;

/*  max and min of power in turn (default 180, -180) */
  double MAXMOMENT = 180;
  double MINMOMENT = -180;

/*  Default port number (default 6000) */
  double PORT = 6000;

/*  Default coach port number (default 6001) */
  double COACH_PORT = 6001;

/*  Default upper limit of the number of online coach's message (default 128) */
  double SAY_COACH_CNT_MAX = 128;

/*  Default upper limit of length of online coach's message (default 128) */
  double SAY_COACH_MSG_SIZE = 128;

/*  Default interval of online coach's look (default 100) */
  double SEND_VI_STEP = 100;

/*  time step of simulation [unit:msec] (default 100) */
  double SIMULATOR_STEP	= 100;

/*  time step of visual information [unit:msec] (default 150) */
  double SEND_STEP = 150;

/*  time step of acception of command [unit: msec] (defalut 20) */
  double RECV_STEP = 10;

/*  length of half of game [unit:sec] (default 6000) */
/*  (if negative, a game does not stop automatically) */
  double HALF_TIME = 300;

/*  string size of say message [unit:byte] (default 256) */
  double SAY_MSG_SIZE = 512;

/*  flag for using off side rule. [on/off] (default on) */
  int USE_OFFSIDE = ON;

/*  offside active area size (default 9.15) */
  double OFFSIDE_ACTIVE_AREA_SIZE = 9.15;

/*  forbid kick off offside. [on/off] (default on)  */
  int FORBID_KICK_OFF_OFFSIDE = ON;

/*  flag for verbose mode. [on/off] (default off) */
  int VERBOSE = OFF;

/*  flag for record log. (default 2)  */
/*  (old version 1) */
  double RECORD_VERSION	= 2;

/*  flag for record client command log. [on/off] (default off) */
  int RECORD_LOG = OFF;

/*  flag for send client command log. [on/off] (default on) */
  int  SEND_LOG = ON;

/*  byte size of Max datagrampacket [unit:byte] (default ???) */
  int MAX_DGPACKET_SIZE = 4096;

}
