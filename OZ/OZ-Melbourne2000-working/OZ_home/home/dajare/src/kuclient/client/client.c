/*
 * �ᵦ��� RoboCup ���ߥ�졼���������
 * ���饤����ȥץ���� 1999
 */
/*
 * client.c
 * ���饤����ȥᥤ��
 *
 * Author: (c) 1999 ���� ͵�� (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include <libsclient.h>

#include "client.h"


Mem mem ;		/* ���饤����ȤγƼ���� */

sigset_t sigio_mask ;	/* �ѥ��åȳ�������ѤΥޥ��� */
sigset_t sigfullmask ;	/* �������Ԥ��ѤΥޥ��� */

/*
 * �����ܡ��ɤ�������Ϥ�������������Ԥ�
 * (�ǽ�Ū�ˤ�sigsupend�����Υ롼�פˤʤ�ͽ��)
 */
void get_key()
{
	while(TRUE)
		sigsuspend(&sigfullmask) ;
}

/*
 * �Ƽ糧�󥵾��󤫤鸽�ߤλ��֤򹹿�
 * ���� time���ƥ��󥵡����������������
 */
void now_time(int time)
{
	if(mem.time < time)
		mem.time = time ;
}

/*
 * info_see���鼫ʬ�ΰ��֤򹹿�
 */
void my_position(int time)
{
	mem.myinfo.px   = mem.pstate.pos.x + mem.myinfo.vx ;
	mem.myinfo.py   = mem.pstate.pos.y + mem.myinfo.vy ;

	mem.myinfo.dir = norm(mem.myinfo.dir + mem.pstate.dir
		- mem.myinfo.hdir + mem.myinfo.ddir) ;
	mem.myinfo.hdir = norm(mem.pstate.dir + mem.myinfo.ddir) ;
}

/*
 * info_see����ܡ���ΰ��֡�®�پ���򹹿�
 */
void ball_position(int i)
{
	double tmp1, tmp2, dist_c, dir_c ;

	dist_c = mem.sensor.info.see.obj[i].dist_chng ;
	dir_c  = mem.sensor.info.see.obj[i].dir_chng ;
	tmp1   = mem.sensor.info.see.obj[i].dist ;
	tmp2   = mem.sensor.info.see.obj[i].dir ;

	if(tmp2 != DIR_ERR) {
		mem.ballinfo.pt = norm(tmp2 + mem.myinfo.hdir - mem.myinfo.ddir) ;

		if(tmp1 != DIST_ERR) {
			mem.ballinfo.pr = tmp1 ;

			rt2xy(mem.ballinfo.pr, mem.ballinfo.pt, &tmp1, &tmp2) ;
			mem.ballinfo.px = tmp1 + mem.myinfo.px - mem.myinfo.vx / PLAYER_DECAY ;
			mem.ballinfo.py = tmp2 + mem.myinfo.py - mem.myinfo.vx / PLAYER_DECAY ;

			if(dist_c != CHNG_ERR && dir_c != CHNG_ERR) {
				mem.ballinfo.f_visible = 3 ;

				sum_vector(  mem.ballinfo.pr + dist_c,  dir_c,
					   - mem.ballinfo.pr         , 0.0,
					    &mem.ballinfo.vr         , &tmp1) ;
				mem.ballinfo.vt = norm(tmp1 + mem.myinfo.hdir - mem.myinfo.ddir) ;

				rt2xy( mem.ballinfo.vr,  mem.ballinfo.vt,
				      &mem.ballinfo.vx, &mem.ballinfo.vy );

				mem.ballinfo.vx += mem.myinfo.vx ;
				mem.ballinfo.vy += mem.myinfo.vy ;
				sum_vector( mem.ballinfo.vr,  mem.ballinfo.vt,
					    mem.myinfo.vr  ,  mem.myinfo.vt,
					   &mem.ballinfo.vr, &mem.ballinfo.vt) ;

				mem.ballinfo.px += mem.ballinfo.vx ;
				mem.ballinfo.py += mem.ballinfo.vy ;
				sum_vector( mem.ballinfo.pr,  mem.ballinfo.pt,
					    mem.ballinfo.vr,  mem.ballinfo.vt,
					   &mem.ballinfo.pr, &mem.ballinfo.pt) ;

				mem.ballinfo.vx *= BALL_DECAY ;
				mem.ballinfo.vy *= BALL_DECAY ;
				mem.ballinfo.vr *= BALL_DECAY ;
			}
			else
				mem.ballinfo.f_visible = 2 ;
			mem.ballinfo.px += mem.ballinfo.vx / BALL_DECAY ;
			mem.ballinfo.py += mem.ballinfo.vy / BALL_DECAY ;
			sum_vector( mem.ballinfo.pr,  mem.ballinfo.pt,
				    mem.ballinfo.vr / BALL_DECAY,  mem.ballinfo.vt,
				   &mem.ballinfo.pr, &mem.ballinfo.pt) ;
			
		}
		else
			mem.ballinfo.f_visible = 1 ;
	}
	else
		mem.ballinfo.f_visible = 0 ;
}

/*
 * info_see����ץ쥤�䡼�ΰ��־���򹹿�
 */
void player_position(int i)
{
	int unum, team ;
	double r, t, x, y ;

	unum = mem.sensor.info.see.obj[i].id.player.unum ;
	r = mem.sensor.info.see.obj[i].dist ;
	t = mem.sensor.info.see.obj[i].dir ;

	if(t != DIR_ERR && r != DIST_ERR) {
		t = norm(t + mem.myinfo.hdir - mem.myinfo.ddir) ;

		rt2xy(r, t, &x, &y) ;
		x += mem.myinfo.px ;
		y += mem.myinfo.py ;

		switch(mem.sensor.info.see.obj[i].id.player.team) {
			case WT_Our :
			mem.our[mem.c_our].unum = unum ;
			mem.our[mem.c_our].px = x ;
			mem.our[mem.c_our].py = y ;
			mem.our[mem.c_our].pr = r ;
			mem.our[mem.c_our].pt = t ;
			mem.c_our ++ ;
			break ;

			case WT_Opponent :
			mem.opponent[mem.c_opponent].unum = unum ;
			mem.opponent[mem.c_opponent].px = x ;
			mem.opponent[mem.c_opponent].py = y ;
			mem.opponent[mem.c_opponent].pr = r ;
			mem.opponent[mem.c_opponent].pt = t ;
			mem.c_opponent ++ ;
			break ;

			case WT_Unknown :
			mem.unknown[mem.c_unknown].unum = unum ;
			mem.unknown[mem.c_unknown].px = x ;
			mem.unknown[mem.c_unknown].py = y ;
			mem.unknown[mem.c_unknown].pr = r ;
			mem.unknown[mem.c_unknown].pt = t ;
			mem.c_unknown ++ ;
		}
	}
}

/*
 * ��о��󤫤饴������������а��֤�׻�
 * ���� i����������ǿ�
 */
void goal_dir(int i)
{
	int tmp ;

	if(mem.sensor.info.see.obj[i].id.side != mem.side)
		tmp = 0 ;
	else if(mem.sensor.info.see.obj[i].id.side == mem.side)
		tmp = 1 ;
	else
		return ;

	if(mem.sensor.info.see.obj[i].dir != DIR_ERR) {
		mem.goalinfo[tmp][1].pt = norm(mem.sensor.info.see.obj[i].dir + mem.myinfo.hdir - mem.myinfo.ddir) ;
		if(mem.sensor.info.see.obj[i].dist != DIST_ERR) {
			mem.goalinfo[tmp][i].pr = mem.sensor.info.see.obj[i].dist ;
			mem.goalinfo[tmp][1].f_visible = 2 ;
		}
		else
			mem.goalinfo[tmp][1].f_visible = 1 ;
	}
}

/*
 * ��о��󤫤饴����ݥ��Ȥ����а��֤�׻�
 * ���� i����������ǿ�
 */
void goalpost_dir(int i)
{
	int team, pos ;

	if(mem.sensor.info.see.obj[i].id.fpos == FP_G_L_T) {
		if(mem.side == S_LEFT) {
			team = 1 ;
			pos = 2 ;
		}
		else {
			team = 0 ;
			pos = 0 ;
		}
	}
	else if(mem.sensor.info.see.obj[i].id.fpos == FP_G_L_B) {
		if(mem.side == S_LEFT) {
			team = 1 ;
			pos = 0 ;
		}
		else {
			team = 0 ;
			pos = 2 ;
		}
	}
	else if(mem.sensor.info.see.obj[i].id.fpos == FP_G_R_T) {
		if(mem.side == S_LEFT) {
			team = 0 ;
			pos = 0 ;
		}
		else {
			team = 1 ;
			pos = 2 ;
		}
	}
	else if(mem.sensor.info.see.obj[i].id.fpos == FP_G_R_T) {
		if(mem.side == S_LEFT) {
			team = 0 ;
			pos = 2 ;
		}
		else {
			team = 1 ;
			pos = 0 ;
		}
	}
	else
		return ;

	if(mem.sensor.info.see.obj[i].dir != DIR_ERR) {
		mem.goalinfo[team][pos].pt = norm(mem.sensor.info.see.obj[i].dir + mem.myinfo.hdir - mem.myinfo.ddir) ;
		if(mem.sensor.info.see.obj[i].dist != DIST_ERR) {
			mem.goalinfo[team][pos].pr = mem.sensor.info.see.obj[i].dist ;
			mem.goalinfo[team][pos].f_visible = 2 ;
		}
		else
			mem.goalinfo[team][pos].f_visible = 1 ;
	}
}

/*
 * ��о��󤫤������ʤ��ä�������������ݥ��Ȥ����а��֤�׻�
 */
void calc_goal_dir(void)
{
	int i , j ;
	double x, y, r, t ;

	for(i = 0; i < 2; i++)
		for(j = 0; j < 3; j++)
			if(mem.goalinfo[i][j].f_visible < 2) {
				if(j == 1)
					y = 0.0 ;
				else {
					y = GOAL_WIDTH / 2.0 ;
					if(i == 0 && j == 0 || i ==1 && j ==2)
						y *= -1.0 ;
				}
				if(i == 0)
					x = PITCH_LENGTH / 2.0 ;
				else
					x = - PITCH_LENGTH / 2.0 ;

				x -= mem.myinfo.px ;
				y -= mem.myinfo.py ;

				xy2rt(x, y, &r, &t) ;

				mem.goalinfo[i][j].pr = r ;

				if(mem.goalinfo[i][j].f_visible == 0)
					mem.goalinfo[i][j].pt = t ;
			}
}

/*
 * info_see���饪�֥������Ȥΰ��֡�®�پ���򹹿�
 */
void obj_position(int time)
{
	int i, j ;

	mem.ballinfo.f_visible = 0 ;
	mem.c_our = 0 ;
	mem.c_opponent = 0 ;
	mem.c_unknown = 0 ;

	for(i=0; i < 2; i++)
		for(j=0; j < 3; j++) {
			mem.goalinfo[i][j].f_visible = 0 ;
		}

	for(i = 0; i < mem.sensor.info.see.n_obj; i++) {
		switch (mem.sensor.info.see.obj[i].type) {

			/*
			 * �ܡ���γƼ�����׻�
			 */
			case OT_Ball :
			ball_position(i) ;
			break ;

			case OT_Player :
			player_position(i) ;
			break ;

			case OT_Goal :
			goal_dir(i) ;
			break ;

			case OT_Flag :
			goalpost_dir(i) ;
			break ;

			case OT_Line :
			break ;

			case OT_Unknown :
			printf("  Obj is Unknown\t") ;
		}
	}

	calc_goal_dir() ;

	if(mem.ballinfo.f_visible == 0) {
		if(mem.ballinfo.c_unvisible < 100)
			mem.ballinfo.c_unvisible ++ ;
	}
	else
		mem.ballinfo.c_unvisible = 0 ;
}

/*
 * sense_body���鼫ʬ�γƼ����򹹿�
 */
void my_states(int time)
{
	double rad ;

	mem.myinfo.hdir = norm(mem.sensor.info.body.head_angle + mem.myinfo.dir) ;
	mem.myinfo.stamina = mem.sensor.info.body.stamina ;
	mem.myinfo.effort  = mem.sensor.info.body.effort ;
	mem.myinfo.vwidth = mem.sensor.info.body.viewwidth ;

	rad = Deg2Rad(mem.myinfo.vt) ;
	mem.myinfo.vr = mem.sensor.info.body.speed ;
	mem.myinfo.vx = mem.myinfo.vr * cos(rad) ;
	mem.myinfo.vy = mem.myinfo.vr * sin(rad) ;

	mem.myinfo.ddir = 0.0 ;
}

/*
 * 100mS���Ȥ˳Ƽ�ͽ¬����򹹿�
 */
void foresee(void) {
	int i, j ;

	/*
	 * �ƥ��֥������Ȥΰ��־���򹹿�
	 */
	mem.myinfo.px += mem.myinfo.vx ;
	mem.myinfo.py += mem.myinfo.vy ;

	mem.ballinfo.px += mem.ballinfo.vx ;
	mem.ballinfo.py += mem.ballinfo.vy ;
	sum_vector(  mem.ballinfo.pr,  mem.ballinfo.pt,
		     mem.ballinfo.vr,  mem.ballinfo.vt,
		    &mem.ballinfo.pr, &mem.ballinfo.pt ) ;
	sum_vector(  mem.ballinfo.pr,  mem.ballinfo.pt,
		   - mem.myinfo.vr,    mem.myinfo.vt,
		    &mem.ballinfo.pr, &mem.ballinfo.pt ) ;

	for(i = 0; i < 2; i++) {
		for(j = 0; j < 3; j++) {
			sum_vector( mem.goalinfo[i][j].pr,  mem.goalinfo[i][j].pt,
					  - mem.myinfo.vr,	    mem.myinfo.vt,
				   &mem.goalinfo[i][j].pr, &mem.goalinfo[i][j].pt ) ;
		}
	}

	/*
	 * �ƥ��֥������Ȥ�®�پ���򹹿�
	 */
	mem.myinfo.vx *= PLAYER_DECAY ;
	mem.myinfo.vy *= PLAYER_DECAY ;
	mem.myinfo.vr *= PLAYER_DECAY ;

	mem.ballinfo.vx *= BALL_DECAY ;
	mem.ballinfo.vy *= BALL_DECAY ;
	mem.ballinfo.vr *= BALL_DECAY ;
}

/*
 * �ѥ��åȼ����ѥ����ʥ�ϥ�ɥ�
 * �ѥ��åȤ��������ȡ���ưŪ�˼¹Ԥ����
 */
void sigio_handler(void)
{
	char buf[MAXMESG] ;

	sigprocmask(SIG_BLOCK, &sigio_mask, NULL) ;

	while(receive_message(&mem.sock, buf, sizeof(buf))) {
		if (scan_info(buf, &mem.sensor, mem.tname)) {
			if (mem.sensor.type == SIT_See) {
				/*
				 * �����ǻ�о���������ν���
				 */
				obj_position(mem.sensor.info.see.time) ;
				if (estimate_current_pos(&mem.sensor.info.see,
							  mem.side, &mem.pstate)) {
					/*
					 * �����Ǽ�ʬ�ΰ��־���������ν���
					 */
					my_position(mem.sensor.info.see.time) ;
				}
				obj_position(mem.sensor.info.see.time) ;
				now_time(mem.sensor.info.see.time) ;
			}
			else if (mem.sensor.type == SIT_Hear) {
				/*
				 * ������İ�о���������ν���
				 */
				if (estimate_current_pm(&mem.sensor.info.hear, &mem.plmode)) {
					/*
					 * �����ǥץ쥤�⡼���ѹ����ν���
					 */
					if(mem.plmode == PM_PlayOn)
						mem.f_haichi = 0 ;
				}
				now_time(mem.sensor.info.hear.time) ;
			}
			else  if (mem.sensor.type == SIT_Body) {
				/*
				 * �����ǥܥǥ�����������ν���(100mS��)
				 */
				my_states(mem.sensor.info.body.time) ;
				handan(&mem) ;	/* Ƚ�ǡ���ư */
				foresee() ;	/* ��ư��γƾ����ͽ¬ */
				now_time(mem.sensor.info.body.time) ;
			}
	}
		else {
			/*
			 * ������ä��ѥ��åȾ���Ƚ���Բ�ǽ���ν���
			 */
			printf("Can't scan_info\n") ;
			printf("%s\n", buf) ;
		}
	}

	sigprocmask(SIG_UNBLOCK, &sigio_mask, NULL) ;
}

/*
 * �ϥ�ɥ������
 */
void init_handler(void)
{
	struct sigaction sigact;

	sigemptyset(&sigio_mask) ;
	sigaddset(&sigio_mask, SIGIO) ;

	sigact.sa_flags = 0 ;
	sigemptyset(&sigact.sa_mask) ;
	sigact.sa_handler = (void (*)(int))sigio_handler ;

	sigaction(SIGIO, &sigact, NULL) ;
	sigprocmask(SIG_UNBLOCK, &sigio_mask, NULL) ;

	sigprocmask(SIG_BLOCK, NULL, &sigfullmask) ;
}

/*
 * ���饤����ȤγƼ��ѿ�������
 */
void init_client(void){
	mem.myinfo.vx = 0.0 ;
	mem.myinfo.vy = 0.0 ;
	mem.myinfo.vr = 0.0 ;
	mem.myinfo.vt = 0.0 ;

	mem.myinfo.dir = 0.0 ;
	mem.myinfo.hdir = 0.0 ;
	mem.maxpower = 100.0 ;

	mem.f_haichi = mem.f_attack = mem.f_defense = mem.f_wait = 0 ;

	mem.ballinfo.px = 0.0 ;
	mem.ballinfo.py = 0.0 ;
	mem.ballinfo.vx = 0.0 ;
	mem.ballinfo.vy = 0.0 ;
	mem.ballinfo.vr = 0.0 ;
	mem.ballinfo.vt = 0.0 ;
	mem.ballinfo.c_unvisible = 9 ;

	mem.c_our = mem.c_opponent = 0 ;

	mem.haichi[ 0].x = - PITCH_LENGTH / 2 + 1.0 ;	mem.haichi[ 0].y =   0.0 ;
	mem.haichi[ 1].x = -20.0 ;	mem.haichi[ 1].y =  25.0 ;
	mem.haichi[ 2].x = -30.0 ;	mem.haichi[ 2].y =  10.0 ;
	mem.haichi[ 3].x = -30.0 ;	mem.haichi[ 3].y = -10.0 ;
	mem.haichi[ 4].x = -20.0 ;	mem.haichi[ 4].y = -25.0 ;
	mem.haichi[ 5].x = -20.0 ;	mem.haichi[ 5].y =   0.0 ;
	mem.haichi[ 6].x = -10.0 ;	mem.haichi[ 6].y =  15.0 ;
	mem.haichi[ 7].x =  -1.0 ;	mem.haichi[ 7].y =   0.0 ;
	mem.haichi[ 8].x = -10.0 ;	mem.haichi[ 8].y = -15.0 ;
	mem.haichi[ 9].x =  -1.0 ;	mem.haichi[ 9].y =  20.0 ;
	mem.haichi[10].x =  -1.0 ;	mem.haichi[10].y = -20.0 ;
}

/*
 * �ᥤ��ؿ�
 * ���ޥ�ɥ饤������� ./sample ������̾ {f,g} [HOST [PORT]]
 */
int main(int argc, char **argv)
{
	char *server, *stile ;
	int port, val ;
	InitInfo info ;
	PlayStile ps ;

	if(argc == 5) {
		strcpy(mem.tname, argv[1]) ;
		stile = argv[2] ;
		server = argv[3] ;
		port = atoi(argv[4]) ;
	} else if (argc == 4) {
		strcpy(mem.tname, argv[1]) ;
		stile = argv[2] ;
		server = argv[3] ;
		port = 6000 ;
	} else if (argc == 3) {
		strcpy(mem.tname, argv[1]) ;
		stile = argv[2] ;
		server = "localhost" ;
		port = 6000 ;
	} else {
		printf("usage: %s TEAMNAME {f,g} [HOST [PORT]]\n", argv[0]) ;
		exit(0) ;
	}

	if(!strcmp(stile, "g"))
		ps = PS_Goalie ;
	else
		ps = PS_Field ;

	mem.sock = init_connection(server,port) ;
	if(mem.sock.socketfd == -1)
		exit(1) ;

	info = send_com_init(&mem.sock, mem.tname, ps) ;

	printf("\ninit!: %d, %d, %s, %d\n\n",
		info.side, info.unum, info.playmodestr, info.playmode) ;

	if (info.side == S_UNKNOWN) {
		printf("Can't init\n") ;
		exit(-1) ;
	}

	mem.plmode = info.playmode ;
	mem.side = info.side ;
	mem.unum = info.unum ;

	init_client() ;
	init_handler() ;

	if (fcntl(mem.sock.socketfd, F_SETOWN, getpid()) < 0)
		exit(1) ;
	val = fcntl(mem.sock.socketfd, F_GETFL, 0) | O_NDELAY | FASYNC ;
	if (fcntl(mem.sock.socketfd, F_SETFL, val) < 0)
		exit(1) ;

	get_key() ;

	close_connection(mem.sock) ;
}

