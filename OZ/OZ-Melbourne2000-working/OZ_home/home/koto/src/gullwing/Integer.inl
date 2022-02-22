// Integer.inl
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
// Integer

inline Integer::Integer()
{
#ifndef NDEBUG
	m_value = INT_MIN;
#endif
}
inline Integer::Integer(int v)
{
	m_value = v;
	ASSERT(valid());
}
inline Integer::operator int () const
{
	ASSERT(valid());
	return m_value;
}

inline Integer Integer::invalidValue()
{
	Integer result;
	result.m_value = INT_MIN;
	return result;
}
inline bool Integer::valid() const
{
	return m_value != INT_MIN;
}

inline bool Integer::operator== (const Integer& x) const
{
	return m_value == x.m_value;
}
inline bool Integer::operator!= (const Integer& x) const
{
	return m_value != x.m_value;
}
inline bool Integer::operator< (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	return m_value < x.m_value;
}
inline bool Integer::operator<= (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	return m_value <= x.m_value;
}
inline bool Integer::operator> (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	return m_value > x.m_value;
}
inline bool Integer::operator>= (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	return m_value >= x.m_value;
}

inline Integer Integer::operator- () const
{
	ASSERT(valid());
	Integer result(-m_value);
	ASSERT(result.valid());
	return result;
}
inline Integer Integer::operator+ () const
{
	ASSERT(valid());
	return Integer(m_value);
}
inline Integer Integer::operator+ (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	Integer result(m_value + x.m_value);
	ASSERT(result.valid());
	return result;
}
inline Integer Integer::operator- (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	Integer result(m_value - x.m_value);
	ASSERT(result.valid());
	return result;
}
inline Integer Integer::operator* (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	Integer result(m_value * x.m_value);
	ASSERT(result.valid());
	return result;
}
inline Integer Integer::operator/ (const Integer& x) const
{
	ASSERT(valid() && x.valid());
	ASSERT(x.m_value != 0);
	Integer result(m_value / x.m_value);
	ASSERT(result.valid());
	return result;
}

inline Integer& Integer::operator++ ()
{
	ASSERT(valid());
	m_value++;
	ASSERT(valid());
	return *this;
}
inline Integer& Integer::operator-- ()
{
	ASSERT(valid());
	m_value--;
	ASSERT(valid());
	return *this;
}
inline Integer Integer::operator++ (int)
{
	ASSERT(valid());
	int old = m_value++;
	return Integer(old);
}
inline Integer Integer::operator-- (int)
{
	ASSERT(valid());
	int old = m_value--;
	return Integer(old);
}
inline Integer& Integer::operator+= (const Integer& x)
{
	ASSERT(valid() && x.valid());
	m_value += x.m_value;
	ASSERT(valid());
	return *this;
}
inline Integer& Integer::operator-= (const Integer& x)
{
	ASSERT(valid() && x.valid());
	m_value -= x.m_value;
	ASSERT(valid());
	return *this;
}
inline Integer& Integer::operator*= (const Integer& x)
{
	ASSERT(valid() && x.valid());
	m_value *= x.m_value;
	ASSERT(valid());
	return *this;
}
inline Integer& Integer::operator/= (const Integer& x)
{
	ASSERT(valid() && x.valid());
	ASSERT(x.m_value != 0);
	m_value /= x.m_value;
	ASSERT(valid());
	return *this;
}


inline Integer abs(Integer v)
{
	ASSERT(v.valid());
#if 1
	int i = v;
	if(i < 0)
		i = -i;
	return i;
#else
	Integer result = (v >= 0) ? v : -v;
	ASSERT(result.valid());
	return result;
#endif
}

inline bool operator== (const Integer& x, int y)
{
	return x == Integer(y);
}
inline bool operator!= (const Integer& x, int y)
{
	return x != Integer(y);
}
inline bool operator< (const Integer& x, int y)
{
	return x < Integer(y);
}
inline bool operator<= (const Integer& x, int y)
{
	return x <= Integer(y);
}
inline bool operator> (const Integer& x, int y)
{
	return x > Integer(y);
}
inline bool operator>= (const Integer& x, int y)
{
	return x >= Integer(y);
}

inline bool operator== (int x, const Integer& y)
{
	return Integer(x) == y;
}
inline bool operator!= (int x, const Integer& y)
{
	return Integer(x) != y;
}
inline bool operator< (int x, const Integer& y)
{
	return Integer(x) < y;
}
inline bool operator<= (int x, const Integer& y)
{
	return Integer(x) <= y;
}
inline bool operator> (int x, const Integer& y)
{
	return Integer(x) > y;
}
inline bool operator>= (int x, const Integer& y)
{
	return Integer(x) >= y;
}

inline Integer operator+ (const Integer& x, int y)
{
	return x + Integer(y);
}
inline Integer operator- (const Integer& x, int y)
{
	return x - Integer(y);
}
inline Integer operator* (const Integer& x, int y)
{
	return x * Integer(y);
}
inline Integer operator/ (const Integer& x, int y)
{
	return x / Integer(y);
}

inline Integer operator+ (int x, const Integer& y)
{
	return Integer(x) + y;
}
inline Integer operator- (int x, const Integer& y)
{
	return Integer(x) - y;
}
inline Integer operator* (int x, const Integer& y)
{
	return Integer(x) * y;
}
inline Integer operator/ (int x, const Integer& y)
{
	return Integer(x) / y;
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
