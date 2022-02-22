/*
 * client.h
 * ¥¯¥é¥¤¥¢¥ó¥È¥Ø¥Ã¥À
 *
 * Author: »³Æâ Íµ²ð (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

/* ºÇÂç¥Ñ¥±¥Ã¥È¥µ¥¤¥º */
#define MAXMESG		2048

/*
 * ¥µ¥Ã¥«¡¼¥µ¡¼¥Ð¤ÎÀßÄêÃÍ¤¬ÊÑ¹¹¤µ¤ì¤¿¾ì¹ç¡¢
 * °Ê²¼¤Î¥Ñ¥é¥á¡¼¥¿ ¤È lÈlibsclient.h ¤Î³Æ¥Ñ¥é¥á¡¼¥¿¤ò
 * ½ñ¤­´¹¤¨¤ëÉ¬Í×¤¬¤¢¤ë¡£
 * ¥Ñ¥é¥á¡¼¥¿¤Î¾ÜºÙ¤Ï¡¢¥µ¥Ã¥«¡¼¥µ¡¼¥Ð¤Î¥Þ¥Ë¥å¥¢¥ë¤ò»²¾È¤·¤ÆÄº¤­¤¿¤¤¡£
 */
#define PLAYER_SIZE	0.3
#define PLAYER_DECAY	0.4
#define PLAYER_SPEED_MAX	1.0

#define STAMINA_MAX	3500.0
#define STAMINA_INC_MAX	35.0

#define INERTIA_MOMENT  5.0

#define BALL_SIZE	0.085
#define BALL_DECAY	0.94
#define BALL_SPEED_MAX	2.7

#define KICKABLE_MARGIN	0.7
#define KICKABLE_AREA	KICKABLE_MARGIN + BALL_SIZE + PLAYER_SIZE

#define DASH_POWER_RATE	0.006
#define KICK_POWER_RATE 0.016

#define MAXPOWER	100.0

/* ¥¢¥¯¥·¥ç¥ó¤Î¼ïÎà */
typedef enum _ActMode {
	AM_None,
	AM_GotoXY, AM_GotoXY2,
	AM_GotoBall, AM_GotoBall2,
	AM_DoubleKick, AM_DoubleKick2,
} ActMode ;

/* 
 * ³Æ¼ï¾ðÊó¤òÊÝ»ý
 */
typedef struct _Mem {
	Socket sock ;		/* ¥½¥±¥Ã¥È */

	char tname[16] ;	/* ¥Á¡¼¥àÌ¾ */
	Side side ;		/* ¥µ¥¤¥É */
	int unum ;		/* ÇØÈÖ¹æ */

	SensorInfo sensor ;	/* ¥»¥ó¥µ¡¼¾ðÊó */


        int sw_1,sw_2,sw_3,p_mode;

	int time ;		/* sense_body ¤Ç¼èÆÀ¤·¤¿»þ´Ö */

	PosState pstate ;	/* ¥Ý¥¸¥·¥ç¥ó xºÂÉ¸ */
	PlayMode plmode ;	/* ¥Ý¥¸¥·¥ç¥ó yºÂÉ¸ */

	double maxpower ;	/* ¼«Ê¬¤ÎºÇÂçÂ®ÅÙ */

	/* ¼«Ê¬¤Î³Æ¼ï¾ðÊó */
	struct MyInfo {
		double px, py ;		/* ÀäÂÐ°ÌÃÖ(x,y) */

		double vx, vy ;		/* ÀäÂÐÂ®ÅÙ(x,y) */
		double vr, vt ;		/* ÀäÂÐÂ®ÅÙ(r,¦È) (¦È¤ÏÀäÂÐ³ÑÅÙ) */

		double dir ;		/* ¸þ¤¤¤Æ¤¤¤ëÊý¸þ (²ÃÂ®¤¹¤ëÊý¸þ) */
		double hdir ;		/* Æ¬¤ÎÀäÂÐ³ÑÅÙ */
		double ddir ;

		ViewWidth vwidth ;	/* »ë³Ð¤Î¹­¤µ */
		double stamina ;	/* ¥¹¥¿¥ß¥Ê */
		double effort ;		/* º¬À­ */
	} myinfo ;

	/* ¥Ü¡¼¥ë¤Î³Æ¼ï¾ðÊó */
	struct BallInfo {
		int f_visible ;		/* ¸«¤¨¤ë:1, ¸«¤¨¤Ê¤¤:0 */
		int c_unvisible ;

		double px, py ;		/* ÀäÂÐ°ÌÃÖ(x,y) */
		double pr, pt ;         /* ÁêÂÐ°ÌÃÖ(r,¦È) (¦È¤ÏÀäÂÐ³ÑÅÙ) */

		double vx, vy ;         /* ÀäÂÐÂ®ÅÙ(x,y) */
		double vr, vt ;		/* ÀäÂÐÂ®ÅÙ(r,¦È) (¦È¤ÏÀäÂÐ³ÑÅÙ) */
	} ballinfo ;

	struct Haichi {
		double x ;
		double y ;
	} haichi[11] ; 

	struct PlyrInfo {
		int unum ;		/* ÇØÈÖ¹æ */

		double px, py ;		/* ÀäÂÐ°ÌÃÖ(x,y) */
		double pr, pt ;		/* ÁêÂÐ°ÌÃÖ(r,¦È) (¦È¤ÏÀäÂÐ³ÑÅÙ) */
	} ;

	struct PlyrInfo our[10] ;	/* Ì£Êý¥×¥ì¥¤¥ä¡¼*/
	struct PlyrInfo opponent[11] ;	/* Å¨¥×¥ì¥¤¥ä¡¼ */
	struct PlyrInfo unknown[21] ;	/* ÉÔÌÀ¥×¥ì¥¤¥ä¡¼ */
	int c_our ;			/* Ì£Êý¥×¥ì¥¤¥ä¡¼¿ô */
	int c_opponent ;		/* Å¨¥×¥ì¥¤¥ä¡¼¿ô */
	int c_unknown ;			/* ÉÔÌÀ¥×¥ì¥¤¥ä¡¼¿ô */

	/*
	 * ¥´¡¼¥ë¤ÎÃæ±û¡¢¥Ý¥¹¥È¤ÎÁêÂÐ°ÌÃÖ
	 * goalinfo[team][pos]
	 * team = 0¡§Å¨, team = 1¡§Ì£Êý
	 * pos = 0¡§º¸¥Ý¥¹¥È, pos = 1¡§Ãæ±û, pos = 2¡§±¦¥Ý¥¹¥È
	 */
	struct GoalInfo {
		int f_visible ;		/* ¸«¤¨¤ë:1, ¸«¤¨¤Ê¤¤:0 */
		double pr, pt ;		/* ÁêÂÐ°ÌÃÖ(r,¦È) */
	} goalinfo[2][3] ;

	/* °Ê²¼¡¢¹ÔÆ°ÍÑ¥Ñ¥é¥á¡¼¥¿ */

	int f_haichi ;
	int f_attack ;
	int f_defense ;
	int f_wait ;

	double ope1, ope2 ;
} Mem ;


/* sendcom.c ¤Ë´Þ¤Þ¤ì¤ë´Ø¿ô */
extern void move(Mem *mem, double x, double y) ;
extern double turn(Mem *mem, double moment) ;
extern double dash(Mem *mem, double power) ;
extern void kick(Mem *mem, double power, double dir) ;
extern double turn_neck(Mem *mem, double dir) ;

/* vector.c ¤Ë´Þ¤Þ¤ì¤ë´Ø¿ô */
extern double pow2(double x) ;
extern double norm(double dir) ;
extern Bool xy2rt(double x, double y, double *r, double *t) ;
extern void rt2xy(double r, double t, double *x, double *y) ;
extern Bool sum_vector(double r1, double t1, double r2, double t2, double *r, double *t) ;

/* action.c ¤Ë´Þ¤Þ¤ì¤ë´Ø¿ô */
extern void handan(Mem *mem) ;
