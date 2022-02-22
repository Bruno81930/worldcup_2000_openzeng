/*
 * sendcom.c
 * ��ư���ޥ�ɤ����С����֥������Ȥ�ͽ¬
 *
 * Author: (c) 1999 ���� ͵�� (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

#include <math.h>

#include <libsclient.h>

#include "client.h"

/*
 * �ࡼ�֥��ޥ�����С���ʬ�ΰ��֤�ͽ¬
 * ���� x, y���ࡼ�ֻ��κ�ɸ
 */
void move(Mem *mem, double x, double y)
{
	int t1 ;

	send_com_move(&mem->sock, x, y);

	mem->myinfo.px = x ;
	mem->myinfo.py = y ;
}

/*
 * �����󥳥ޥ�����С���ʬ�θ����Ƥ�����������ž�Ѳ��̤�ͽ¬
 * ���� dir�������������г���(-180 ~ 180)
 * ����͡��ºݤ˥�����Ǥ������г���
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
 * ���å��女�ޥ�����С���ʬ�β�®�١�®�٤�ͽ¬
 * ���� power: ���å�����Υѥ(-100 ~ 100)
 * ����͡���®�٤��礭��
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
 * ���å����ޥ�ɤ����С����å���Υܡ����®�٤�ͽ¬
 * ���� speed�����å���Υ��ԡ���, dir�����å��������г���
 */
void kick2(Mem *mem, double speed, double dir)
{
	double pr, pt, kick_dir, power, kick_accel, rad ;

	pr = mem->ballinfo.pr ;
	pt = mem->ballinfo.pt ;

	/* �ºݤ˽���������®�٤η׻� */
	sum_vector(speed, dir, - mem->ballinfo.vr, mem->ballinfo.vt, &kick_accel, &kick_dir) ;

	/* �����Ϥη׻� */
	power = kick_accel / KICK_POWER_RATE / (1.0 
		- 0.25 * Abs(norm(pt - mem->myinfo.dir)) / 180.0
		- 0.25 * (pr - PLAYER_SIZE - BALL_SIZE) / KICKABLE_AREA) ;

	/* �����Ϥ������ͤ�ۤ��Ƥ�����ν��� */
	if(power > 100.0) {
		power = 100.0 ;
		kick_accel = power * KICK_POWER_RATE * (1.0
			- 0.25 * Abs(norm(pt - mem->myinfo.dir)) / 180.0
			- 0.25 * (pr - PLAYER_SIZE - BALL_SIZE) / KICKABLE_AREA) ;
	}
	/* ���ޥ������ */
	send_com_kick(&mem->sock, power, norm(kick_dir - mem->myinfo.dir)) ;

	/* ͽ¬
	if(pr < KICKABLE_AREA) {
		rad = Deg2Rad(kick_dir) ;
		mem->ballinfo.vx += kick_accel * cos(rad) ;
		mem->ballinfo.vy += kick_accel * sin(rad) ;
		sum_vector( mem->ballinfo.vr,  mem->ballinfo.vt, kick_accel, kick_dir,
			   &mem->ballinfo.vr, &mem->ballinfo.vt);
	} */
}

/*
 * ������ͥå����ޥ�ɤ����С�Ƭ�����г��٤�׻�
 * ���� dir��������ͥå�������г���
 * ����͡��ºݤ˲�ž�Ǥ������г���
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

