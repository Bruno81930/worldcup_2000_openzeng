/*
 * client.h
 * ���饤����ȥإå�
 *
 * Author: ���� ͵�� (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

/* ����ѥ��åȥ����� */
#define MAXMESG		2048

/*
 * ���å��������Ф������ͤ��ѹ����줿��硢
 * �ʲ��Υѥ�᡼�� �� l�libsclient.h �γƥѥ�᡼����
 * �񤭴�����ɬ�פ����롣
 * �ѥ�᡼���ξܺ٤ϡ����å��������ФΥޥ˥奢��򻲾Ȥ���ĺ��������
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

/* ���������μ��� */
typedef enum _ActMode {
	AM_None,
	AM_GotoXY, AM_GotoXY2,
	AM_GotoBall, AM_GotoBall2,
	AM_DoubleKick, AM_DoubleKick2,
} ActMode ;

/* 
 * �Ƽ������ݻ�
 */
typedef struct _Mem {
	Socket sock ;		/* �����å� */

	char tname[16] ;	/* ������̾ */
	Side side ;		/* ������ */
	int unum ;		/* ���ֹ� */

	SensorInfo sensor ;	/* ���󥵡����� */


        int sw_1,sw_2,sw_3,p_mode;

	int time ;		/* sense_body �Ǽ����������� */

	PosState pstate ;	/* �ݥ������ x��ɸ */
	PlayMode plmode ;	/* �ݥ������ y��ɸ */

	double maxpower ;	/* ��ʬ�κ���®�� */

	/* ��ʬ�γƼ���� */
	struct MyInfo {
		double px, py ;		/* ���а���(x,y) */

		double vx, vy ;		/* ����®��(x,y) */
		double vr, vt ;		/* ����®��(r,��) (�Ȥ����г���) */

		double dir ;		/* �����Ƥ������� (��®��������) */
		double hdir ;		/* Ƭ�����г��� */
		double ddir ;

		ViewWidth vwidth ;	/* ��Фι��� */
		double stamina ;	/* �����ߥ� */
		double effort ;		/* ���� */
	} myinfo ;

	/* �ܡ���γƼ���� */
	struct BallInfo {
		int f_visible ;		/* ������:1, �����ʤ�:0 */
		int c_unvisible ;

		double px, py ;		/* ���а���(x,y) */
		double pr, pt ;         /* ���а���(r,��) (�Ȥ����г���) */

		double vx, vy ;         /* ����®��(x,y) */
		double vr, vt ;		/* ����®��(r,��) (�Ȥ����г���) */
	} ballinfo ;

	struct Haichi {
		double x ;
		double y ;
	} haichi[11] ; 

	struct PlyrInfo {
		int unum ;		/* ���ֹ� */

		double px, py ;		/* ���а���(x,y) */
		double pr, pt ;		/* ���а���(r,��) (�Ȥ����г���) */
	} ;

	struct PlyrInfo our[10] ;	/* ̣���ץ쥤�䡼*/
	struct PlyrInfo opponent[11] ;	/* Ũ�ץ쥤�䡼 */
	struct PlyrInfo unknown[21] ;	/* �����ץ쥤�䡼 */
	int c_our ;			/* ̣���ץ쥤�䡼�� */
	int c_opponent ;		/* Ũ�ץ쥤�䡼�� */
	int c_unknown ;			/* �����ץ쥤�䡼�� */

	/*
	 * �������������ݥ��Ȥ����а���
	 * goalinfo[team][pos]
	 * team = 0��Ũ, team = 1��̣��
	 * pos = 0�����ݥ���, pos = 1�����, pos = 2�����ݥ���
	 */
	struct GoalInfo {
		int f_visible ;		/* ������:1, �����ʤ�:0 */
		double pr, pt ;		/* ���а���(r,��) */
	} goalinfo[2][3] ;

	/* �ʲ�����ư�ѥѥ�᡼�� */

	int f_haichi ;
	int f_attack ;
	int f_defense ;
	int f_wait ;

	double ope1, ope2 ;
} Mem ;


/* sendcom.c �˴ޤޤ��ؿ� */
extern void move(Mem *mem, double x, double y) ;
extern double turn(Mem *mem, double moment) ;
extern double dash(Mem *mem, double power) ;
extern void kick(Mem *mem, double power, double dir) ;
extern double turn_neck(Mem *mem, double dir) ;

/* vector.c �˴ޤޤ��ؿ� */
extern double pow2(double x) ;
extern double norm(double dir) ;
extern Bool xy2rt(double x, double y, double *r, double *t) ;
extern void rt2xy(double r, double t, double *x, double *y) ;
extern Bool sum_vector(double r1, double t1, double r2, double t2, double *r, double *t) ;

/* action.c �˴ޤޤ��ؿ� */
extern void handan(Mem *mem) ;
