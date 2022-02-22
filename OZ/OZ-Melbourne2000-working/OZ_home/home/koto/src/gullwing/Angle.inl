// Angle.inl
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
// Angle

inline Angle::Angle()
{
#ifndef NDEBUG
	m_degrees = Double::invalidValue();
#endif
}
inline Angle::Angle(Double degrees)
{
	ASSERT(degrees.valid());
	while(degrees <= -180.0)
		degrees += 360.0;
	while(degrees > +180.0)
		degrees -= 360.0;
	m_degrees = degrees;
	ASSERT(valid());
}
inline Angle Angle::fromRadians(double radians)
{
	return Angle(radians * (180.0 / PI));
}

inline Angle Angle::invalidValue()
{
	Angle result;
	result.m_degrees = Double::invalidValue();
	return result;
}
inline bool Angle::valid() const
{
	return m_degrees.valid();
}

inline double Angle::degrees() const
{
	ASSERT(valid());
	return (double)m_degrees;
}
inline double Angle::radians() const
{
	ASSERT(valid());
	return (double)m_degrees * (PI/180.0);
}

inline bool Angle::operator== (Angle x) const
{
	return m_degrees == x.m_degrees;
}
inline bool Angle::operator!= (Angle x) const
{
	return m_degrees != x.m_degrees;
}
/*inline bool Angle::operator< (Angle x) const
{
	ASSERT(valid() && x.valid());
	return m_degrees < x.m_degrees;
}
inline bool Angle::operator<= (Angle x) const
{
	ASSERT(valid() && x.valid());
	return m_degrees <= x.m_degrees;
}
inline bool Angle::operator> (Angle x) const
{
	ASSERT(valid() && x.valid());
	return m_degrees > x.m_degrees;
}
inline bool Angle::operator>= (Angle x) const
{
	ASSERT(valid() && x.valid());
	return m_degrees >= x.m_degrees;
}*/

inline const Angle& Angle::operator+ () const
{
	ASSERT(valid());
	return *this;
}
inline Angle Angle::operator- () const
{
	ASSERT(valid());
	return Angle(-m_degrees);
}
inline Angle Angle::operator+ (Angle x) const
{
	ASSERT(valid() && x.valid());
	return Angle(m_degrees + x.m_degrees);
}
inline Angle Angle::operator- (Angle x) const
{
	ASSERT(valid() && x.valid());
	return Angle(m_degrees - x.m_degrees);
}
/*inline Angle Angle::operator* (double x) const
{
	ASSERT(valid());
	ASSERT(-180.0 < m_degrees * x && m_degrees * x <= 180.0);
	return Angle(m_degrees * x);
}
inline Angle Angle::operator/ (double x) const
{
	ASSERT(valid());
	ASSERT(x != 0);
	ASSERT(-180.0 < m_degrees / x && m_degrees / x <= 180.0);
	return Angle(m_degrees / x);
}*/

inline Angle& Angle::operator+= (Angle x)
{
	ASSERT(valid() && x.valid());
	m_degrees += x.m_degrees;
	return *this;
}
inline Angle& Angle::operator-= (Angle x)
{
	ASSERT(valid() && x.valid());
	m_degrees -= x.m_degrees;
	return *this;
}
/*inline Angle& Angle::operator*= (double x)
{
	ASSERT(valid());
	ASSERT(-180.0 < m_degrees * x && m_degrees * x <= 180.0);
	m_degrees *= x;
	return *this;
}
inline Angle& Angle::operator/= (double x)
{
	ASSERT(valid());
	ASSERT(x != 0);
	ASSERT(-180.0 < m_degrees / x && m_degrees / x <= 180.0);
	m_degrees /= x;
	return *this;
}*/

inline Angle abs(Angle angle)
{
	return Angle(abs(angle.degrees()));
}
inline double sin(Angle theta)
{
	ASSERT(theta.valid());
	return sin(theta.radians());
}
inline double cos(Angle theta)
{
	ASSERT(theta.valid());
	return cos(theta.radians());
}
inline double tan(Angle theta)
{
	ASSERT(theta.valid());
	return tan(theta.radians());
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
