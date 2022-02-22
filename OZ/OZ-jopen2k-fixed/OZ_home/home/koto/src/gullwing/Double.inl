// Double.inl
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
// Double

inline Double::Double()
{
#ifndef NDEBUG
	m_value = -DBL_MAX;
#endif
}
inline Double::Double(double v)
{
	m_value = v;
	ASSERT(valid());
}
inline Double::Double(const Integer& v)
{
	ASSERT(v.valid());
	m_value = (int)v;
	ASSERT(valid());
}
inline Double::operator double () const
{
	ASSERT(valid());
	return m_value;
}

inline Double Double::invalidValue()
{
	Double result;
	result.m_value = -DBL_MAX;
	return result;
}
inline bool Double::valid() const
{
	return m_value != -DBL_MAX;
}

inline bool Double::operator== (const Double& x) const
{
	return m_value == x.m_value;
}
inline bool Double::operator!= (const Double& x) const
{
	return m_value != x.m_value;
}
inline bool Double::operator< (const Double& x) const
{
	ASSERT(valid() && x.valid());
	return m_value < x.m_value;
}
inline bool Double::operator<= (const Double& x) const
{
	ASSERT(valid() && x.valid());
	return m_value <= x.m_value;
}
inline bool Double::operator> (const Double& x) const
{
	ASSERT(valid() && x.valid());
	return m_value > x.m_value;
}
inline bool Double::operator>= (const Double& x) const
{
	ASSERT(valid() && x.valid());
	return m_value >= x.m_value;
}

inline Double Double::operator- () const
{
	ASSERT(valid());
	Double result(-m_value);
	ASSERT(result.valid());
	return result;
}
inline Double Double::operator+ () const
{
	ASSERT(valid());
	return Double(m_value);
}
inline Double Double::operator+ (const Double& x) const
{
	ASSERT(valid() && x.valid());
	Double result(m_value + x.m_value);
	ASSERT(result.valid());
	return result;
}
inline Double Double::operator- (const Double& x) const
{
	ASSERT(valid() && x.valid());
	Double result(m_value - x.m_value);
	ASSERT(result.valid());
	return result;
}
inline Double Double::operator* (const Double& x) const
{
	ASSERT(valid() && x.valid());
	Double result(m_value * x.m_value);
	ASSERT(result.valid());
	return result;
}
inline Double Double::operator/ (const Double& x) const
{
	ASSERT(valid() && x.valid());
	ASSERT(x.m_value != 0);
	Double result(m_value / x.m_value);
	ASSERT(result.valid());
	return result;
}

inline Double& Double::operator+= (const Double& x)
{
	ASSERT(valid() && x.valid());
	m_value += x.m_value;
	ASSERT(valid());
	return *this;
}
inline Double& Double::operator-= (const Double& x)
{
	ASSERT(valid() && x.valid());
	m_value -= x.m_value;
	ASSERT(valid());
	return *this;
}
inline Double& Double::operator*= (const Double& x)
{
	ASSERT(valid() && x.valid());
	m_value *= x.m_value;
	ASSERT(valid());
	return *this;
}
inline Double& Double::operator/= (const Double& x)
{
	ASSERT(valid() && x.valid());
	ASSERT(x.m_value != 0);
	m_value /= x.m_value;
	ASSERT(valid());
	return *this;
}

inline Double abs(Double v)
{
	ASSERT(v.valid());
#if 1
	double d = v;
	if(d < 0)
		d = -d;
	return d;
#else
	Double result = (v >= 0) ? v : -v;
	ASSERT(result.valid());
	return result;
#endif
}

inline bool operator== (const Double& x, double y)
{
	return x == Double(y);
}
inline bool operator!= (const Double& x, double y)
{
	return x != Double(y);
}
inline bool operator< (const Double& x, double y)
{
	return x < Double(y);
}
inline bool operator<= (const Double& x, double y)
{
	return x <= Double(y);
}
inline bool operator> (const Double& x, double y)
{
	return x > Double(y);
}
inline bool operator>= (const Double& x, double y)
{
	return x >= Double(y);
}

#if !defined(EGCSBUG)
inline bool operator== (double x, const Double& y)
{
	return Double(x) == y;
}
inline bool operator!= (double x, const Double& y)
{
	return Double(x) != y;
}
inline bool operator< (double x, const Double& y)
{
	return Double(x) < y;
}
inline bool operator<= (double x, const Double& y)
{
	return Double(x) <= y;
}
inline bool operator> (double x, const Double& y)
{
	return Double(x) > y;
}
inline bool operator>= (double x, const Double& y)
{
	return Double(x) >= y;
}
#endif // !defined(EGCSBUG)

inline Double operator+ (const Double& x, double y)
{
	return x + Double(y);
}
inline Double operator- (const Double& x, double y)
{
	return x - Double(y);
}
inline Double operator* (const Double& x, double y)
{
	return x * Double(y);
}
inline Double operator/ (const Double& x, double y)
{
	return x / Double(y);
}

#if !defined(EGCSBUG)
inline Double operator+ (double x, const Double& y)
{
	return Double(x) + y;
}
inline Double operator- (double x, const Double& y)
{
	return Double(x) - y;
}
inline Double operator* (double x, const Double& y)
{
	return Double(x) * y;
}
inline Double operator/ (double x, const Double& y)
{
	return Double(x) / y;
}
#endif // !defined(EGCSBUG)


inline bool operator== (const Double& x, int y)
{
	return x == Double(y);
}
inline bool operator!= (const Double& x, int y)
{
	return x != Double(y);
}
inline bool operator< (const Double& x, int y)
{
	return x < Double(y);
}
inline bool operator<= (const Double& x, int y)
{
	return x <= Double(y);
}
inline bool operator> (const Double& x, int y)
{
	return x > Double(y);
}
inline bool operator>= (const Double& x, int y)
{
	return x >= Double(y);
}

#if !defined(EGCSBUG)
inline bool operator== (int x, const Double& y)
{
	return Double(x) == y;
}
inline bool operator!= (int x, const Double& y)
{
	return Double(x) != y;
}
inline bool operator< (int x, const Double& y)
{
	return Double(x) < y;
}
inline bool operator<= (int x, const Double& y)
{
	return Double(x) <= y;
}
inline bool operator> (int x, const Double& y)
{
	return Double(x) > y;
}
inline bool operator>= (int x, const Double& y)
{
	return Double(x) >= y;
}
#endif // !defined(EGCSBUG)

inline Double operator+ (const Double& x, int y)
{
	return x + Double(y);
}
inline Double operator- (const Double& x, int y)
{
	return x - Double(y);
}
inline Double operator* (const Double& x, int y)
{
	return x * Double(y);
}
inline Double operator/ (const Double& x, int y)
{
	return x / Double(y);
}

#if !defined(EGCSBUG)
inline Double operator+ (int x, const Double& y)
{
	return Double(x) + y;
}
inline Double operator- (int x, const Double& y)
{
	return Double(x) - y;
}
inline Double operator* (int x, const Double& y)
{
	return Double(x) * y;
}
inline Double operator/ (int x, const Double& y)
{
	return Double(x) / y;
}
#endif !defined(EGCSBUG)

/////////////////////////////////////////////////////////////////////////////
#undef inline
