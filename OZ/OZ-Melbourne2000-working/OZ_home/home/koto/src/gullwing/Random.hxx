// Random.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(RANDOM_HXX__INCLUDED) // {
#define RANDOM_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Random

/** 乱数生成クラス */
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
	/** ０以上１未満の乱数を返す。*/
	double next();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Random.inl"
#endif

#endif // } !defined(RANDOM_HXX__INCLUDED)
