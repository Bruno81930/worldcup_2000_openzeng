// Server.inl
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
// Server

inline int Server::sayBits() const
{
	if(m_sayBits < 0)
		m_sayBits = getBits(strlen(SAY_MESSAGE_LETTERS())) - 1;
	return m_sayBits;
}
inline double Server::kickable_area() const {
	return kickable_margin() + player_size() + ball_size();
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
