#include "mymath.h"
#include "vector.h"

// 初期化
Vector::Vector(double x, double y)
{
  this->x = x;
  this->y = y;
}


// 距離
double Vector::distance(Vector a)
{
  return (a - *this).r();
}


// 方角
double Vector::direction(Vector a)
{
  return (a - *this).th();
}


// 回転
Vector Vector::rotate(Vector orig, double th)
{
  Vector tmp(*this - orig);

  return Vector(tmp.x * Cos(th) - tmp.y * Sin(th), 
		tmp.x * Sin(th) + tmp.y * Cos(th)) + orig;
}


// 演算Vector += Vector;
void Vector::operator+=(const Vector& a)
{
  x += a.x;
  y += a.y;
}


// 演算Vector -= Vector;
void Vector::operator-=(const Vector& a)
{
  x -= a.x;
  y -= a.y;
}


// 演算Vector *= double;
void Vector::operator*=(const double& a)
{
  x *= a;
  y *= a;
}


// 演算Vector /= double;
void Vector::operator/=(const double& a)
{
  x /= a;
  y /= a;
}


// 演算Vector + Vector;
Vector operator+(const Vector& a, const Vector& b)
{
  return Vector(a.x + b.x, a.y + b.y);
}


// 演算Vector - Vector;
Vector operator-(const Vector& a, const Vector& b)
{
  return Vector(a.x - b.x, a.y - b.y);
}


// 演算Vector * double;
Vector operator*(const Vector& a, const double& b)
{
  return Vector(a.x * b, a.y * b);
}


// 演算Vector / Vector;
Vector operator/(const Vector& a, const double& b)
{
  return Vector(a.x / b, a.y / b);
}


// 極座標からベクトル座標への変換
Vector polar2vector(double r, double th)
{
  return Vector(r * Cos(th), r * Sin(th));
}
