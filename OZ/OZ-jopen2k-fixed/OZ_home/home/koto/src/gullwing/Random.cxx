// Random.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "Random.hxx"

#ifndef NDEBUG
#  include "Random.inl"
#endif

#if OS_TYPE == WINDOWS
#include <process.h>
inline int getpid() {
	return _getpid();
}
#elif OS_TYPE == UNIX
#else
#  error
#endif

/////////////////////////////////////////////////////////////////////////////
// Random

#if MULTI_THREAD
long Random::m_metaSeed = 0;
#endif

Random::Random()
{
	ASSERT(CHAR_BIT >= 8 && sizeof(m_seed) >= 4);
	time_t now;
	time(&now);
	long seed = (long)now * (long)getpid();
#if MULTI_THREAD
	if(m_metaSeed == 0) {
		m_metaSeed = seed;
	}
	m_metaSeed = m_metaSeed * 1103515245L + 12345;
	m_seed = (m_metaSeed >> 16) & 0x7fff;
#else
	m_seed = seed;
#endif
	next();
}
