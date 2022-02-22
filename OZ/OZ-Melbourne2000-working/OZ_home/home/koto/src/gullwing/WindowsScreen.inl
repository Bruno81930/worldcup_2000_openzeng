// WindowsScreen.inl
//
/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Server.hxx"
#endif

#ifndef NDEBUG
#  define inline
#else
#  define inline inline
#endif
/////////////////////////////////////////////////////////////////////////////
// WindowsScreen

inline WindowsScreen::WindowsScreen()
{
	m_hdcCompatible = ::CreateCompatibleDC(NULL);
	m_hbitmapOriginal = NULL;
	
	m_hbrushCurrentColor = NULL;
	m_hpenCurrentColor = NULL;
	
	m_screenWidth = m_screenHeight = 0;
	m_width = m_height = 1;
	m_ratioX = m_ratioY = 1;
	m_baseX = m_baseY = 0;
}

inline void WindowsScreen::set(int width, int height)
{
	m_width = server().PITCH_LENGTH() + 8.0 * 2;
	m_height = server().PITCH_WIDTH() + 8.0 * 2;
	
	m_screenWidth = width;
	m_screenHeight = height;
	
	m_ratioX = (double)width / m_width;
	m_ratioY = (double)height / m_height;
	m_ratioX = m_ratioY = min(m_ratioX, m_ratioY);
	m_baseX = m_screenWidth/2;
	m_baseY = m_screenHeight/2;
	
	HDC hdc = ::GetDC(::GetDesktopWindow());
	HBITMAP hbitmap = ::CreateCompatibleBitmap(hdc, m_screenWidth, m_screenHeight);
	ASSERT(hbitmap);
	if(!hbitmap)
		return;
	if(m_hbitmapOriginal == NULL) {
		m_hbitmapOriginal = ::SelectObject(m_hdcCompatible, hbitmap);
		ASSERT(m_hbitmapOriginal);
		if(!m_hbitmapOriginal) {
			::DeleteObject(hbitmap);
		}
	} else {
		HGDIOBJ hbitmapOld = ::SelectObject(m_hdcCompatible, hbitmap);
		ASSERT(hbitmapOld);
		if(hbitmapOld)
			::DeleteObject(hbitmapOld);
		else
			::DeleteObject(hbitmap);
	}
	::ReleaseDC(::GetDesktopWindow(), hdc);
}

inline void WindowsScreen::drawTo(HDC hdc)
{
	::BitBlt(hdc, 0, 0, m_screenWidth, m_screenHeight, m_hdcCompatible, 0, 0, SRCCOPY);
}

inline int WindowsScreen::x(double v)
{
	return (int)(v * m_ratioX + m_baseX);
}
inline int WindowsScreen::y(double v)
{
	return (int)(v * m_ratioY + m_baseY);
}
inline int WindowsScreen::w(double v)
{
	return (int)(v * m_ratioX);
}
inline int WindowsScreen::h(double v)
{
	return (int)(v * m_ratioY);
}

/////////////////////////////////////////////////////////////////////////////
#undef inline
