// Blue.inl
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
// Blue

inline Blue::Blue()
{
	m_fp = NULL;
}

/*inline Blue::Blue(Blue const& source)
{
	Blue();
	operator= (source);
}
inline Blue& Blue::operator= (Blue const& lhs)
{
	if(this == &lhs)
		return *this;
	return *this;
}
inline bool Blue::operator== (Blue const& lhs) const
{
	if(this == &lhs)
		return true;
}*/

/////////////////////////////////////////////////////////////////////////////
#undef inline
