#include <math.h>
#define PI M_PI
#define RAD2DEG	(180.0 / PI )
#define DEG2RAD (PI / 180.0)
#define Rad2Deg(a) 	((double)(a) * RAD2DEG)
#define Deg2Rad(a)	((double)(a) * DEG2RAD)
#define Sin(x)     (sin(Deg2Rad(x)))
#define Cos(x)     (cos(Deg2Rad(x)))
#define Tan(x)     (tan(Deg2Rad(x)))
#define Asin(x)    Rad2Deg(asin(x))
#define Atan(y, x) Rad2Deg(atan2((y), (x)))
