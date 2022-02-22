/*
 * vector.c
 * �٥��ȥ�黻���δؿ�
 *
 * Author: ���� ͵�� (Yusuke Yamauchi)
 * Date: 1999/06/20
 */

#include <math.h>

#include <libsclient.h>

/*
 * ����黻
 */
double pow2(double x)
{
	return x * x ;
}

/*
 * ���٤�����������
 * ���� dir������
 * ����͡����������줿����
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
 * x,y��ɸ�Ϥ�r,����ɸ�Ϥ��Ѵ�
 * ����  x,  y��x, y��ɸ
 *      *r, *t��r,����ɸ
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
 * r,����ɸ�Ϥ�x,y��ɸ�Ϥ��Ѵ�
 * ���� *r, *t��r,����ɸ
 *      x,  y��x, y��ɸ
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
