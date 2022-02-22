// Random.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(RANDOM_HXX__INCLUDED) // {
#define RANDOM_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Random

/** $BMp?t@8@.%/%i%9(B */
class Random
{
private:
	Random(const Random&);
	Random& operator= (const Random&);
	bool operator== (const Random&) const;

protected:
#if MULTI_THREAD
	static long m_metaSeed;
#endif
	long m_seed;

public:
	Random();
	/** $B#00J>e#1L$K~$NMp?t$rJV$9!#(B*/
	double next();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Random.inl"
#endif

#endif // } !defined(RANDOM_HXX__INCLUDED)
