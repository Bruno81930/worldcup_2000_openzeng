/*
 * sendcom.c
 * 行動コマンドの送出、オブジェクトの予測
 *
 * Author: (c) 1999 山内 裕介 (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

#include <math.h>

#include <libsclient.h>

#include "client.h"

/*
 * ムーブコマンド送出、自分の位置を予測
 * 引数 x, y：ムーブ時の座標
 */
void move(Mem *mem, double x, double y)
{
	int t1 ;

	send_com_move(&mem->sock, x, y);

	mem->myinfo.px = x ;
	mem->myinfo.py = y ;
}

/*
 * ターンコマンド送出、自分の向いている方向、回転変化量を予測
 * 引数 dir：ターン後の絶対角度(-180 ~ 180)
 * 戻り値：実際にターンできた絶対角度
 */
double turn(Mem *mem, double dir)
{
	double ang, moment ;

	ang = norm(dir - mem->myinfo.dir) ;
	moment = ang * (1.0 + INERTIA_MOMENT * mem->myinfo.vr) ;

	if (Abs(moment) > 180.0) {
		if(moment > 0.0)
			moment = 180.0 ;
		else
			moment = -180.0 ;

		ang = moment / (1.0 + INERTIA_MOMENT * mem->myinfo.vr) ;
		mem->myinfo.dir = norm(mem->myinfo.dir + ang) ;
	}
	else
		mem->myinfo.dir = norm(dir) ;

	send_com_turn(&mem->sock, moment) ;

	mem->myinfo.hdir = norm(mem->myinfo.hdir + ang) ;
	mem->myinfo.ddir = ang ;

	return mem->myinfo.dir ;
}

/*
 * ダッシュコマンド送出、自分の加速度、速度を予測
 * 引数 power: ダッシュ時のパワー(-100 ~ 100)
 * 戻り値：加速度の大きさ
 */
double dash(Mem *mem, double power)
{
	double rate, vr, vt, ar, rad ;

	if(power > 100.0)
		power = 100.0 ;
	else if(power < -100.0)
		power = - 100.0 ;

	if(power > mem->myinfo.stamina)
		power = mem->myinfo.stamina ;
	else if( - power > mem->myinfo.stamina)
		power = - 0.5 * mem->myinfo.stamina ;

	rate = DASH_POWER_RATE * mem->myinfo.effort ;

	ar = power * rate ;

	sum_vector(mem->myinfo.vr, mem->myinfo.vt, ar, mem->myinfo.dir, &vr, &vt) ;
	if(vr > PLAYER_SPEED_MAX) {
		rad = Deg2Rad(norm(mem->myinfo.vt - mem->myinfo.dir)) ;
		if(power > 0)
			power = - mem->myinfo.vr * cos(rad)
				+ sqrt(pow2(PLAYER_SPEED_MAX) -
					pow2(mem->myinfo.vr * sin(rad))) / rate ;
		else
			power = - mem->myinfo.vr * cos(rad)
				- sqrt(pow2(PLAYER_SPEED_MAX) -
					pow2(mem->myinfo.vr * sin(rad))) / rate ;
		ar = PLAYER_SPEED_MAX ;
	}

	send_com_dash(&mem->sock, power) ;

	rad = Deg2Rad(mem->myinfo.dir) ;

	mem->myinfo.vx += ar * cos(rad) ;
	mem->myinfo.vy += ar * sin(rad) ;

	sum_vector( mem->myinfo.vr,  mem->myinfo.vt, ar, mem->myinfo.dir,
		   &mem->myinfo.vr, &mem->myinfo.vt) ;

	return ar ;
}

void kick(Mem *mem, double power, double dir)
{
	send_com_kick(&mem->sock, power, norm(dir - mem->myinfo.dir)) ;
}

/*
 * キックコマンドの送出、キック後のボールの速度を予測
 * 引数 speed：キック後のスピード, dir：キックする絶対角度
 */
void kick2(Mem *mem, double speed, double dir)
{
	double pr, pt, kick_dir, power, kick_accel, rad ;

	pr = mem->ballinfo.pr ;
	pt = mem->ballinfo.pt ;

	/* 実際に蹴る方向、速度の計算 */
	sum_vector(speed, dir, - mem->ballinfo.vr, mem->ballinfo.vt, &kick_accel, &kick_dir) ;

	/* 蹴る力の計算 */
	power = kick_accel / KICK_POWER_RATE / (1.0 
		- 0.25 * Abs(norm(pt - mem->myinfo.dir)) / 180.0
		- 0.25 * (pr - PLAYER_SIZE - BALL_SIZE) / KICKABLE_AREA) ;

	/* 蹴る力が最大値を越えている場合の処理 */
	if(power > 100.0) {
		power = 100.0 ;
		kick_accel = power * KICK_POWER_RATE * (1.0
			- 0.25 * Abs(norm(pt - mem->myinfo.dir)) / 180.0
			- 0.25 * (pr - PLAYER_SIZE - BALL_SIZE) / KICKABLE_AREA) ;
	}
	/* コマンド送出 */
	send_com_kick(&mem->sock, power, norm(kick_dir - mem->myinfo.dir)) ;

	/* 予測
	if(pr < KICKABLE_AREA) {
		rad = Deg2Rad(kick_dir) ;
		mem->ballinfo.vx += kick_accel * cos(rad) ;
		mem->ballinfo.vy += kick_accel * sin(rad) ;
		sum_vector( mem->ballinfo.vr,  mem->ballinfo.vt, kick_accel, kick_dir,
			   &mem->ballinfo.vr, &mem->ballinfo.vt);
	} */
}

/*
 * ターンネックコマンドの送出、頭の絶対角度を計算
 * 引数 dir：ターンネック後の絶対角度
 * 戻り値：実際に回転できた絶対角度
 */
double turn_neck(Mem *mem, double dir)
{
	double neck ;

	neck = norm(dir - mem->myinfo.dir) ;
	if(Abs(neck) > 89.0){
		if(neck > 0)
			neck = 89.0 ;
		else
			neck = - 89.0 ;
		dir = norm(mem->myinfo.dir + neck) ;
	}
		
	send_com_turn_neck(&mem->sock, norm(dir - mem->myinfo.hdir)) ;

	mem->myinfo.hdir = norm(dir) ;

	return dir ;
}

