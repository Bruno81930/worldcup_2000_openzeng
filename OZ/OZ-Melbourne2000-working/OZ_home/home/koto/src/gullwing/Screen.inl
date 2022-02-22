// Screen.inl
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
// Screen

inline Screen::Screen()
{
}

inline void Screen::color(int rgb)
{
	color((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}

/////////////////////////////////////////////////////////////////////////////
#undef inline