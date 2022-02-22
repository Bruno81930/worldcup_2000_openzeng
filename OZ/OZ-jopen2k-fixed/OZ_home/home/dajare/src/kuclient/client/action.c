/*
 * KU-Sakura2
 * action.c
 * (c) 1999 Yusuke Yamauchi
 */

#include <math.h>
#include <libsclient.h>

#include "client.h"

void down_line(Mem *mem)
{
	double tmp ;

	if(mem->f_wait == 0) {
		if(mem->unum > 1)
			tmp = - 180.0 ;
		else
			tmp = norm(mem->goalinfo[1][1].pt) ;
		if(turn(mem, tmp) == tmp)
			mem->f_wait = 1 ;
	}
	else {
		if(mem->unum > 1)
			tmp = STAMINA_INC_MAX * 2.0 ;
		else
			tmp = STAMINA_INC_MAX * 2.0 ; 
		dash(mem, tmp) ;
		mem->f_wait = 0 ;
	}
}

void up_line(Mem *mem)
{
	if(mem->f_wait == 0) {
		if(turn(mem, 0.0) == 0.0)
			mem->f_wait = 1 ;
	}
	else {
		dash(mem, 100.0) ;
		mem->f_wait = 0 ;
	}
}

void haichi(Mem *mem)
{
	double dist, dir ;

	if(mem->f_haichi == 0 && mem->unum > 1)
		mem->unum = (mem->unum + 2) % 10 + 2 ;

	if(mem->f_haichi < 2) {
		move(mem, mem->haichi[mem->unum - 1].x, mem->haichi[mem->unum - 1].y) ;
		mem->f_haichi ++ ;
	}
	else {
		xy2rt(- mem->haichi[mem->unum - 1].x, - mem->haichi[mem->unum - 1].y,
		      &dist, &dir) ;
		turn(mem, dir) ;

		mem->myinfo.vx = 0.0 ;
		mem->myinfo.vy = 0.0 ;
		mem->myinfo.vr = 0.0 ;

		mem->ballinfo.px = 0.0 ;
		mem->ballinfo.py = 0.0 ;
		mem->ballinfo.vx = 0.0 ;
		mem->ballinfo.vy = 0.0 ;
		mem->ballinfo.vr = 0.0 ;
		
		mem->ballinfo.c_unvisible = 9 ;
	}
}

/*
 * ダブルキック
 * ope1: キックする方向(絶対角度)
 */
void doublekick(Mem *mem)
{
	double tmp ;

	if(mem->f_attack == 1) {
		kick(mem, 100.0, mem->ope1) ;
		mem->f_attack = 0 ;
	}
	else {
		kick(mem, 30.0, mem->ope1) ;
		mem->f_attack = 1 ;
	}
}

/*
 * ボールの方向を向く
 */
void turnball(Mem *mem)
{
	double dist, dir ;

	if(mem->ballinfo.c_unvisible > 2)
		dir = norm(mem->myinfo.dir + 45.0) ;
	else {
		dir = mem->ballinfo.pt ;
	}

	turn(mem, dir) ;
}

/*
 * ボールを追いかける
 */
void gotoball(Mem *mem)
{
	int i, tmp ;
	double dir, dist, tmp2, tmp3 ;

	sum_vector(mem->ballinfo.pr, mem->ballinfo.pt,
		   mem->ballinfo.vr , mem->ballinfo.vt,
		   &dist, &dir);
	sum_vector(dist, dir,
		   - mem->myinfo.vr ,
		   mem->myinfo.vt,
		   &dist, &dir) ;

	/*tmp2 = mem->ballinfo.vr * sin(Deg2Rad(mem->ballinfo.vt - mem->ballinfo.pt)) ;
	if(Abs(tmp2) >= 0.9)
		dir = mem->ballinfo.pt ;
	else
		dir = norm(mem->ballinfo.pt + Rad2Deg(asin(tmp2 / 0.9))) ; */

	if(norm(dir - mem->myinfo.dir) > 0.0)
		dir += 5.0 ;
	else
		dir -= 5.0 ;

	/* ボールの方向を向いていない、かつ、ダッシュ直後ならターンモードにする */
	tmp2 = norm(dir - mem->myinfo.dir) ;
	if(Abs(tmp2) > 10.0 && mem->f_defense == 2) {
		mem->f_defense = 0 ;
		dir = norm(dir) ;
	}

	/* ターンモード時、またはダッシュ直後 */
	if(mem->c_opponent > 0 && (mem->f_defense == 2)) {
		/* 敵が邪魔なら交わす */
		tmp = 0 ;
		for(i = 1 ; i < mem->c_opponent ; i++)
			if(mem->opponent[i].pr < mem->opponent[tmp].pr)
				tmp = i ;
		tmp2 = Rad2Deg(asin(PLAYER_SIZE * 2.0 / mem->opponent[tmp].pr)) ;
		tmp3 = norm(mem->opponent[tmp].pt - dir) ;
		if(Abs(tmp3) < tmp2) {
			if(tmp3 > 0.0)
				tmp2 = - tmp2 ;
			dir = norm(mem->opponent[tmp].pt + tmp2) ;
			mem->f_defense = 0 ;
		}
	}

	/* ターンモード時にターンをする */
	if(mem->f_defense == 0) {
		if(turn(mem, dir) == dir)
			mem->f_defense = 1 ;
	}
	/* ターンモードでない場合は、ダッシュをする */
	else {
		/*tmp2 = (dist - PLAYER_SIZE - BALL_SIZE) / (1.0 + PLAYER_DECAY)
			/ DASH_POWER_RATE / mem->myinfo.effort ;
		if(tmp2 > mem->myinfo.stamina)
			tmp2 = mem->myinfo.stamina ;
		else if(tmp2 > 100.0)
			tmp2 = 100.0 ; */
		dash(mem, 100.0) ;
		mem->f_defense = 2 ;
	}
}

/****************************************************************************************
 *											*
 *	以下、行動を選択								*
 *											*
 ****************************************************************************************/

void wait(Mem *mem)
{
	mem->f_attack = mem->f_defense = 0 ;

	if(mem->unum == 1 && mem->myinfo.px > - PITCH_LENGTH / 2 + 2.0)
		down_line(mem);
	else if(mem->ballinfo.c_unvisible < 3 && mem->ballinfo.px < mem->myinfo.px &&
		mem->myinfo.px > mem->haichi[mem->unum-1].x)
		down_line(mem) ;
	else if(mem->ballinfo.c_unvisible < 3 && mem->ballinfo.px > mem->myinfo.px &&
		mem->myinfo.stamina > STAMINA_MAX - 100.0 &&
		mem->myinfo.px < mem->haichi[mem->unum-1].x + 30.0 && mem->unum != 1)
		up_line(mem) ;
	else
		turnball(mem) ;
}

void defense(Mem *mem)
{
	mem->f_attack = mem->f_wait = 0 ;

	gotoball(mem) ;
}

void attack(Mem *mem)
{
	int i, j, center, num[11], tmp ;
	double left, right, tmp2 ;

	mem->f_defense = mem->f_wait = 0 ;

	/************************************************************************
	 *	ダブルキック中ならキックを続ける				*
	 ************************************************************************/
	if(mem->f_attack == 2) {
		doublekick(mem) ;

		return ;
	}


	/************************************************************************
	 *	シュート可能かどうか						*
	 ************************************************************************/

	if(mem->myinfo.px > PITCH_LENGTH / 2 - PENALTY_AREA_LENGTH &&
	   Abs(mem->myinfo.py) < PENALTY_AREA_WIDTH / 2) {
		if(mem->myinfo.py < 0.0)
			mem->ope1 = norm(mem->goalinfo[0][0].pt + 1.0) ;
		else
			mem->ope1 = norm(mem->goalinfo[0][2].pt - 1.0) ;
		doublekick(mem) ;
		printf("shot2\n") ;

		return ;
	}

	if(norm(mem->myinfo.hdir - mem->myinfo.ddir + 90) > 0.0) {

		/* 敵を左に見えるほうからソート */
		for(i = 0 ; i < mem->c_opponent ; num[i] = i++) ;
		for(i = 1 ; i < mem->c_opponent ; i++) {
			tmp = num[i] ;
			for(j = i ; j > 0 &&
			    norm(mem->opponent[num[j-1]].pt - mem->goalinfo[0][1].pt)
			    > norm(mem->opponent[tmp].pt - mem->goalinfo[0][1].pt) ;
			    num[j] = num [--j]) ;
			num[j] = tmp ;
		}
		/* 一番ゴールの中心側に見える敵を探す */
		for(tmp = 0 ; tmp < mem->c_opponent &&
		    norm(mem->opponent[num[tmp]].pt - mem->goalinfo[0][1].pt) < 0.0 ; tmp++) ;

		/* シュートコースの計算 */
		left = right = mem->goalinfo[0][1].pt ;
		if(tmp >= mem->c_opponent)
			center = mem->c_opponent - 1 ;
		else
			center = tmp ;
		for(i = center ; i >= 0 ; i--) {
			if(norm(mem->opponent[num[i]].pt - right - 10.0) < 0.0)
				right = norm(mem->opponent[num[i]].pt + 10.0) ;
			if(norm(mem->opponent[num[i]].pt - left + 10.0) > 0.0)
				left = norm(mem->opponent[num[i]].pt - 10.0) ;
			else
				break ;
		}
		if(tmp <= 0)
			center = 1 ;
		else
			center = tmp ;
		for(i = center - 1 ; i < mem->c_opponent ; i++) {
			if(norm(mem->opponent[num[i]].pt - left + 10.0) > 0.0)
				left = norm(mem->opponent[num[i]].pt - 10.0) ;
			if(norm(mem->opponent[num[i]].pt - right - 10.0) < 0.0)
				 right = norm(mem->opponent[num[i]].pt + 10.0) ;
			else
				break ;
		}
		if(norm(left  - mem->goalinfo[0][0].pt) >
		   norm(mem->goalinfo[0][2].pt - right)  ) {
			tmp2 = left ;
			mem->ope1 = norm(mem->goalinfo[0][0].pt) + 1.0 ;
		}
		else {
			tmp2 = right ;
			mem->ope1 = norm(mem->goalinfo[0][2].pt) - 1.0 ;
		}

		/* 計算したコースでシュート可能かどうか */
		if(norm(tmp2 - mem->goalinfo[0][0].pt) > 0.0 &&
		   norm(tmp2 - mem->goalinfo[0][2].pt) < 0.0   ) {
			if(mem->myinfo.px > 0.0)
				mem->ope1 = tmp2 ;
			doublekick(mem) ;
			printf("shot1\n") ;

			return ;
		}
	}

	/************************************************************************
	 *	パス可能か							*
	 ************************************************************************/

	/* 味方をゴール方向に近い順にソート */
	/*
	for(i = 0 ; i < mem->c_our ; num[i] = i++) ;
	for(i = 1 ; i < mem->c_our ; i++) {
		tmp = num[i] ;
		for(j = i ; j > 0 &&
		    Abs(norm(mem->our[num[j-1]].pt - mem->goalinfo[0][1].pt))
		    > Abs(norm(mem->our[tmp].pt - mem->goalinfo[0][1].pt)) ;
		    num[j] = num [--j]) ;
		num[j] = tmp ;
	}
	*/
	/* パス可能な味方が存在するか */
	/*
	for(i = 0 ; i < mem->c_our ; i++) {
		for(j = 0 ; j < mem->c_opponent ; j++)
			if(Abs(norm(mem->our[num[i]].pt - mem->opponent[j].pt)) < 30.0
			   && mem->our[num[i]].pr > mem->opponent[j].pr - 10.0
			   && mem->our[num[i]].pr < 10.0)
				break ;
		if(j == mem->c_opponent)
			break ;
	}
	if(i != mem->c_our) {
	*/
		/* 味方の方向がゴール方向ならパスする */
		/*
		if(Abs(norm(mem->our[num[i]].pt - mem->goalinfo[0][1].pt)) < 90.0) {
			kick(mem, 100.0, mem->our[num[i]].pt) ;
			mem->f_attack = 0 ;
			printf("pass\n") ;

			return ;
		}
	}
	*/

	/************************************************************************
	 *	clear								*
	 ************************************************************************/

	if(Abs(mem->myinfo.py) > PENALTY_AREA_WIDTH / 2)
		if(mem->myinfo.px > PITCH_LENGTH / 2 - PENALTY_AREA_LENGTH) {
			xy2rt(PITCH_LENGTH / 2 - PENALTY_AREA_LENGTH - mem->myinfo.px,
			      - mem->myinfo.py, &tmp2, &mem->ope1) ;
			printf("centering\n") ;
		}
		else {
			kick(mem, 100.0, 0.0) ;
			printf("clear1\n") ;
		}
	else {
		if(mem->myinfo.py > 0.0)
			kick(mem, 100.0, 30.0) ;
		else
			kick(mem, 100.0, -30.0) ;
		printf("clear2\n") ;
	}
	mem->f_attack = 0 ;
}

/****************************************************************************************
 *											*
 *	戦略モードを選択								*
 *											*
 ****************************************************************************************/
void handan(Mem *mem)
{
	int i ;
	double dist, dir, tmp2 ;

	/* キックオフ前 */
	if(mem->plmode == PM_BeforeKickOff ||
	   mem->plmode == PM_Goal_L ||
	   mem->plmode == PM_Goal_R) {
		haichi(mem) ;
		return ;
	}
	else if(mem->plmode != PM_PlayOn &&
		mem->plmode != PM_FreeKick_L &&
		mem->plmode != PM_FreeKick_R &&
		mem->f_haichi < 6) {
		haichi(mem) ;
		mem->f_haichi ++ ;
		return ;
	}

	/* 相手のフリーキック時 */
	if(mem->side == S_RIGHT && (
		mem->plmode == PM_KickOff_L ||
		mem->plmode == PM_KickIn_L ||
		mem->plmode == PM_FreeKick_L ||
		mem->plmode == PM_CornerKick_L ||
		mem->plmode == PM_GoalKick_L
	   ) ||
	   mem->side == S_LEFT && (
		mem->plmode == PM_KickOff_R ||
		mem->plmode == PM_KickIn_R ||
		mem->plmode == PM_FreeKick_R ||
		mem->plmode == PM_CornerKick_R ||
		mem->plmode == PM_GoalKick_R
	   )) {
		wait(mem) ;
		return ;
	}

	if(Abs(norm(mem->ballinfo.pt - mem->myinfo.dir)) > 25.0 &&
	   mem->sensor.info.body.viewwidth != VW_Wide)
		send_com_change_view(&mem->sock, VW_Wide, VQ_high) ;
	else if(Abs(norm(mem->ballinfo.pt - mem->myinfo.dir)) < 25.0 &&
	   mem->sensor.info.body.viewwidth != VW_Normal)
		send_com_change_view(&mem->sock, VW_Normal, VQ_high) ;

	if(mem->unum == 1 && mem->ballinfo.pr < 2.0 && mem->f_attack == 0
	   && mem->ballinfo.c_unvisible < 3) {
		send_com_catch(&mem->sock, norm(mem->ballinfo.pt - mem->myinfo.dir)) ;
		mem->f_attack = -1 ;

		return ;
	}

	if(mem->f_attack == -1 && mem->unum == 1) {
		if(mem->plmode == PM_FreeKick_L && mem->side == S_LEFT ||
		   mem->plmode == PM_FreeKick_R && mem->side == S_RIGHT) {
			if(mem->myinfo.py < 0)
				move(mem, PENALTY_AREA_LENGTH - PITCH_LENGTH / 2 - 1.0, 
				     PENALTY_AREA_WIDTH / 2.0 - 1.0) ;
			else
				move(mem, PENALTY_AREA_LENGTH - PITCH_LENGTH / 2 - 1.0,
				     - PENALTY_AREA_WIDTH / 2.0 + 1.0) ;
			mem->f_attack = -2 ;

			mem->ballinfo.pr = mem->ballinfo.pt = 0.0 ;
			mem->ballinfo.vx = mem->ballinfo.vy = 0.0 ;
			mem->ballinfo.vr = mem->ballinfo.vt = 0.0 ;
			mem->ballinfo.px = mem->myinfo.px ;
			mem->ballinfo.py = mem->myinfo.py ;

			return ;
		}
		else
			mem->f_attack = 0 ;
	}

	if(mem->f_attack > -5 && mem->f_attack < -1 && mem->unum == 1) {
		turn(mem, 0.0) ;
		mem->f_attack -- ;
		if(mem->f_attack == -5)
			mem->f_attack = 1 ;

		return ;
	}

	if(mem->f_attack == 0 && mem->ballinfo.pr < KICKABLE_AREA
	   && mem->ballinfo.c_unvisible < 3)
		mem->f_attack = 1 ;

	if(mem->f_attack > 0) {
		attack(mem) ;
		return ;
	}

	if(mem->ballinfo.px < PENALTY_AREA_LENGTH - PITCH_LENGTH / 2 &&
	   Abs(mem->ballinfo.py) < PENALTY_AREA_WIDTH / 2 &&
	   mem->ballinfo.c_unvisible < 3) {
		if(mem->unum == 1)
			defense(mem) ;
		else
			wait(mem) ;

		return ;
	}

	if(mem->unum > 1 && mem->myinfo.stamina >= STAMINA_MAX * 0.3 + MAXPOWER &&
	   mem->ballinfo.c_unvisible < 3) {
		for(i = 0 ; i < mem->c_our ; i++) {
			sum_vector(mem->ballinfo.pr, mem->ballinfo.pt,
				   - mem->our[i].pr, mem->our[i].pt, &dist, &dir) ;
			if(dist < mem->ballinfo.pr - 5.0 && mem->our[1].unum != 1)
				break ;
		}
		if(i == mem->c_our) {
			defense(mem) ;
			return ;
		}
	} 

	wait(mem) ;
}
