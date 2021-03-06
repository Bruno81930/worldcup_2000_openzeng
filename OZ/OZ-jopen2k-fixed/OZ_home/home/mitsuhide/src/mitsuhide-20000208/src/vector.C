#include "mymath.h"
#include "vector.h"

// $B=i4|2=(B
Vector::Vector(double x, double y)
{
  this->x = x;
  this->y = y;
}


// $B5wN%(B
double Vector::distance(Vector a)
{
  return (a - *this).r();
}


// $BJ}3Q(B
double Vector::direction(Vector a)
{
  return (a - *this).th();
}


// $B2sE>(B
Vector Vector::rotate(Vector orig, double th)
{
  Vector tmp(*this - orig);

  return Vector(tmp.x * Cos(th) - tmp.y * Sin(th), 
		tmp.x * Sin(th) + tmp.y * Cos(th)) + orig;
}


// $B1i;;(BVector += Vector;
void Vector::operator+=(const Vector& a)
{
  x += a.x;
  y += a.y;
}


// $B1i;;(BVector -= Vector;
void Vector::operator-=(const Vector& a)
{
  x -= a.x;
  y -= a.y;
}


// $B1i;;(BVector *= double;
void Vector::operator*=(const double& a)
{
  x *= a;
  y *= a;
}


// $B1i;;(BVector /= double;
void Vector::operator/=(const double& a)
{
  x /= a;
  y /= a;
}


// $B1i;;(BVector + Vector;
Vector operator+(const Vector& a, const Vector& b)
{
  return Vector(a.x + b.x, a.y + b.y);
}


// $B1i;;(BVector - Vector;
Vector operator-(const Vector& a, const Vector& b)
{
  return Vector(a.x - b.x, a.y - b.y);
}


// $B1i;;(BVector * double;
Vector operator*(const Vector& a, const double& b)
{
  return Vector(a.x * b, a.y * b);
}


// $B1i;;(BVector / Vector;
Vector operator/(const Vector& a, const double& b)
{
  return Vector(a.x / b, a.y / b);
}


// $B6K:BI8$+$i%Y%/%H%k:BI8$X$NJQ49(B
Vector polar2vector(double r, double th)
{
  return Vector(r * Cos(th), r * Sin(th));
}
