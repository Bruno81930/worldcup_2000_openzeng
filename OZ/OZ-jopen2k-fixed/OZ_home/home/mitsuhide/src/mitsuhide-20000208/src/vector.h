class Vector {
  friend class WorldModel;
  friend class Ball;
  friend class Self;
  friend class Player;

private:
  double x;
  double y;

public:
  Vector(double x = 0.0, double y = 0.0);
  double get_x(void) { return x; };
  double get_y(void) { return y; };
  void set_x(double x) { this->x = x; };
  void set_y(double y) { this->y = y; };
  double r(void) { return sqrt(pow(x, 2) + pow(y, 2)); };
  double th(void) { return Atan(y, x); };
  double distance(Vector a);
  double direction(Vector a);
  Vector rotate(Vector orig, double th);
  void operator+=(const Vector& a);
  void operator-=(const Vector& a);
  void operator*=(const double& a);
  void operator/=(const double& a);
  friend Vector operator+(const Vector& a, const Vector& b);
  friend Vector operator-(const Vector& a, const Vector& b);
  friend Vector operator*(const Vector& a, const double& b);
  friend Vector operator/(const Vector& a, const double& b);
};

Vector polar2vector(double r, double th);
