/*
 * vector.c
 * ベクトル演算等の関数
 *
 * Author: 山内 裕介 (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

#include <math.h>

#include <libsclient.h>

/*
 * 自乗演算
 */
double pow2(double x)
{
	return x * x ;
}

/*
 * 角度を正規化する
 * 引数 dir：角度
 * 戻り値：正規化された角度
 */
double norm(double dir)
{
	while(dir >= 180.0)
		dir -= 360.0 ;
	while(dir < -180.0)
		dir += 360.0 ;

	return dir ;
}

/*
 * x,y座標系をr,Θ座標系に変換
 * 引数  x,  y：x, y座標
 *      *r, *t：r,Θ座標
 */
Bool xy2rt(double x, double y, double *r, double *t)
{
	if(x == 0.0 && y == 0.0) {
		*r = *t = 0;

		return FALSE ;
	}
	else {
		*r= sqrt(x * x + y * y) ;
		if(Abs(x) > Abs(y)) {
			*t = Rad2Deg(asin(y / *r)) ;
			if(x < 0)
				*t = 180.0 - *t ;
		}
		else {
			*t = Rad2Deg(acos(x / *r)) ;
			if(y < 0)
				*t = - *t ;
		}
		*t = norm(*t) ;

		return TRUE ;
	}
}

/*
 * r,Θ座標系をx,y座標系に変換
 * 引数 *r, *t：r,Θ座標
 *      x,  y：x, y座標
 */
void rt2xy(double r, double t, double *x, double *y)
{
	t = Deg2Rad(t) ;

	*x = r * cos(t) ;
	*y = r * sin(t) ;
}

Bool sum_vector(double r1, double t1, double r2, double t2, double *r, double *t)
{
	double x, y, rad1, rad2 ;

	*r = pow2(r1) + pow2(r2) + 2 * r1 * r2 * cos(Deg2Rad(t1 - t2)) ;

	if(*r == 0) {
		*t = 0 ;

		return FALSE ;
	}
	else {
		*r = sqrt(*r) ;

		rad1 = Deg2Rad(t1) ;
		rad2 = Deg2Rad(t2) ;

		x = r1 * cos(rad1) + r2 * cos(rad2) ;
		y = r1 * sin(rad1) + r2 * sin(rad2) ;

		if(Abs(x) > Abs(y)) {
			*t = Rad2Deg(asin(y / *r)) ;
			if(x < 0)
				*t = 180.0 - *t ;
		}
		else {
			*t = Rad2Deg(acos(x / *r)) ;
			if(y < 0)
				*t = - *t ;
		}
		*t = norm(*t) ;

		return TRUE ;
	}	
}
