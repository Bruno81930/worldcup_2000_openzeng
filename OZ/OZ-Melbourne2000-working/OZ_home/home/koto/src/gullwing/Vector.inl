// Vector.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
// #include
#endif

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// Vector

inline Vector::Vector()
{
#ifndef NDEBUG
	m_x = Double::invalidValue();
	m_y = Double::invalidValue();
#endif
}
inline Vector::Vector(const Double& x, const Double& y)
{
	ASSERT(x.valid() && y.valid());
	m_x = x;
	m_y = y;
	ASSERT(valid());
}
inline Vector::Vector(const Double& r, Angle theta)
{
	ASSERT(r.valid() && theta.valid());
	m_x = r * cos(theta);
	m_y = r * sin(theta);
	ASSERT(valid());
}

inline Vector Vector::invalidValue()
{
	Vector result;
	result.m_x = Double::invalidValue();
	result.m_y = Double::invalidValue();
	return result;
}
inline bool Vector::valid() const
{
	return m_x != Double::invalidValue() && m_y != Double::invalidValue();
}

inline double Vector::x() const
{
	ASSERT(valid());
	return m_x;
}
inline double Vector::y() const
{
	ASSERT(valid());
	return m_y;
}
inline Angle Vector::angle() const
{
	ASSERT(valid());
	return Angle::fromRadians(atan2((double)m_y, (double)m_x));
}
inline double Vector::norm() const
{
	ASSERT(valid());
	double x = (double)m_x;
	double y = (double)m_y;
	return sqrt(x*x + y*y);
}

inline bool Vector::operator== (Vector x) const
{
	return m_x == x.m_x && m_y == x.m_y;
}
inline bool Vector::operator!= (Vector x) const
{
	return !(*this == x);
}

inline Vector Vector::operator- () const
{
	ASSERT(valid());
	return Vector(-m_x, -m_y);
}
inline Vector Vector::operator+ () const
{
	ASSERT(valid());
	return Vector(m_x, m_y);
}
inline Vector Vector::operator+ (Vector x) const
{
	ASSERT(valid() && x.valid());
	return Vector(m_x + x.m_x, m_y + x.m_y);
}
inline Vector Vector::operator- (Vector x) const
{
	ASSERT(valid() && x.valid());
	return Vector(m_x - x.m_x, m_y - x.m_y);
}
inline Vector Vector::operator* (double x) const
{
	ASSERT(valid());
	return Vector(m_x * x, m_y * x);
}
inline Vector Vector::operator/ (double x) const
{
	ASSERT(valid());
	ASSERT(x != 0);
	return Vector(m_x / x, m_y / x);
}

inline Vector& Vector::operator+= (Vector x)
{
	ASSERT(valid() && x.valid());
	m_x += x.m_x;
	m_y += x.m_y;
	return *this;
}
inline Vector& Vector::operator-= (Vector x)
{
	ASSERT(valid() && x.valid());
	m_x -= x.m_x;
	m_y -= x.m_y;
	return *this;
}
inline Vector& Vector::operator*= (double x)
{
	ASSERT(valid());
	m_x *= x;
	m_y *= x;
	return *this;
}
inline Vector& Vector::operator/= (double x)
{
	ASSERT(valid());
	ASSERT(x != 0);
	m_x /= x;
	m_y /= x;
	return *this;
}
inline Vector& Vector::normalize(double x)
{
	ASSERT(valid());
	ASSERT(norm() != 0);
	return *this *= (x / norm());
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
