/*
 * client.h
 * クライアントヘッダ
 *
 * Author: 山内 裕介 (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

/* 最大パケットサイズ */
#define MAXMESG		2048

/*
 * サッカーサーバの設定値が変更された場合、
 * 以下のパラメータ と l?libsclient.h の各パラメータを
 * 書き換える必要がある。
 * パラメータの詳細は、サッカーサーバのマニュアルを参照して頂きたい。
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

/* アクションの種類 */
typedef enum _ActMode {
	AM_None,
	AM_GotoXY, AM_GotoXY2,
	AM_GotoBall, AM_GotoBall2,
	AM_DoubleKick, AM_DoubleKick2,
} ActMode ;

/* 
 * 各種情報を保持
 */
typedef struct _Mem {
	Socket sock ;		/* ソケット */

	char tname[16] ;	/* チーム名 */
	Side side ;		/* サイド */
	int unum ;		/* 背番号 */

	SensorInfo sensor ;	/* センサー情報 */

	int time ;		/* sense_body で取得した時間 */

	PosState pstate ;	/* ポジション x座標 */
	PlayMode plmode ;	/* ポジション y座標 */

	double maxpower ;	/* 自分の最大速度 */

	/* 自分の各種情報 */
	struct MyInfo {
		double px, py ;		/* 絶対位置(x,y) */

		double vx, vy ;		/* 絶対速度(x,y) */
		double vr, vt ;		/* 絶対速度(r,θ) (θは絶対角度) */

		double dir ;		/* 向いている方向 (加速する方向) */
		double hdir ;		/* 頭の絶対角度 */
		double ddir ;

		ViewWidth vwidth ;	/* 視覚の広さ */
		double stamina ;	/* スタミナ */
		double effort ;		/* 根性 */
	} myinfo ;

	/* ボールの各種情報 */
	struct BallInfo {
		int f_visible ;		/* 見える:1, 見えない:0 */
		int c_unvisible ;

		double px, py ;		/* 絶対位置(x,y) */
		double pr, pt ;         /* 相対位置(r,θ) (θは絶対角度) */

		double vx, vy ;         /* 絶対速度(x,y) */
		double vr, vt ;		/* 絶対速度(r,θ) (θは絶対角度) */
	} ballinfo ;

	struct Haichi {
		double x ;
		double y ;
	} haichi[11] ; 

	struct PlyrInfo {
		int unum ;		/* 背番号 */

		double px, py ;		/* 絶対位置(x,y) */
		double pr, pt ;		/* 相対位置(r,θ) (θは絶対角度) */
	} ;

	struct PlyrInfo our[10] ;	/* 味方プレイヤー*/
	struct PlyrInfo opponent[11] ;	/* 敵プレイヤー */
	struct PlyrInfo unknown[21] ;	/* 不明プレイヤー */
	int c_our ;			/* 味方プレイヤー数 */
	int c_opponent ;		/* 敵プレイヤー数 */
	int c_unknown ;			/* 不明プレイヤー数 */

	/*
	 * ゴールの中央、ポストの相対位置
	 * goalinfo[team][pos]
	 * team = 0：敵, team = 1：味方
	 * pos = 0：左ポスト, pos = 1：中央, pos = 2：右ポスト
	 */
	struct GoalInfo {
		int f_visible ;		/* 見える:1, 見えない:0 */
		double pr, pt ;		/* 相対位置(r,θ) */
	} goalinfo[2][3] ;

	/* 以下、行動用パラメータ */

	int f_haichi ;
	int f_attack ;
	int f_defense ;
	int f_wait ;

	double ope1, ope2 ;
} Mem ;


/* sendcom.c に含まれる関数 */
extern void move(Mem *mem, double x, double y) ;
extern double turn(Mem *mem, double moment) ;
extern double dash(Mem *mem, double power) ;
extern void kick(Mem *mem, double power, double dir) ;
extern double turn_neck(Mem *mem, double dir) ;

/* vector.c に含まれる関数 */
extern double pow2(double x) ;
extern double norm(double dir) ;
extern Bool xy2rt(double x, double y, double *r, double *t) ;
extern void rt2xy(double r, double t, double *x, double *y) ;
extern Bool sum_vector(double r1, double t1, double r2, double t2, double *r, double *t) ;

/* action.c に含まれる関数 */
extern void handan(Mem *mem) ;
