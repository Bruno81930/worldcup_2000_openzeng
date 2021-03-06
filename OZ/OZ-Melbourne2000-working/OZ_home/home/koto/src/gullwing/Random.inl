// Random.inl
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
// Random

inline double Random::next()
{
	m_seed = m_seed * 214013L + 2531011L;
	return (1.0 / 0x8000) * ((m_seed >> 16) & 0x7fff);
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
