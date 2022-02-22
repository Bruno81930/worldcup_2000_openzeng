// WindowsScreen.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#if WITH_WINDOW && OS_TYPE == WINDOWS // {
#include "WindowsScreen.hxx"

#include "Server.hxx"

#ifndef NDEBUG
#  include "WindowsScreen.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// WindowsScreen

WindowsScreen::~WindowsScreen()
{
	if(m_hbitmapOriginal) {
		ASSERT(m_hdcCompatible);
		HGDIOBJ hBitmap = ::SelectObject(m_hdcCompatible, m_hbitmapOriginal);
		ASSERT(hBitmap);
		::DeleteObject(hBitmap);
	}
	if(m_hdcCompatible)
		::DeleteDC(m_hdcCompatible);
	
	if(m_hbrushCurrentColor)
		::DeleteObject(m_hbrushCurrentColor);
	if(m_hpenCurrentColor)
		::DeleteObject(m_hpenCurrentColor);
}

int WindowsScreen::width() const
{
	return m_screenWidth;
}
int WindowsScreen::height() const
{
	return m_screenHeight;
}

void WindowsScreen::fillBackground()
{
	int result;
	
	RECT rectScreen = { -1, -1, m_screenWidth+1, m_screenHeight+1 };
	result = ::FillRect(m_hdcCompatible, &rectScreen, m_hbrushCurrentColor);
	ASSERT(result);
}
void WindowsScreen::color(int r, int g, int b)
{
	if(m_hbrushCurrentColor)
		::DeleteObject(m_hbrushCurrentColor);
	if(m_hpenCurrentColor)
		::DeleteObject(m_hpenCurrentColor);
	
	COLORREF cref = RGB(r, g, b);
	m_hbrushCurrentColor = ::CreateSolidBrush(cref);
	m_hpenCurrentColor = ::CreatePen(PS_SOLID, 0, cref);
}
void WindowsScreen::line(double x0, double y0, double x1, double y1)
{
	HGDIOBJ old = ::SelectObject(m_hdcCompatible, m_hpenCurrentColor);
	
	BOOL result = ::MoveToEx(m_hdcCompatible, x(x0), y(y0), NULL);
	ASSERT(result);
	result = ::LineTo(m_hdcCompatible, x(x1), y(y1));
	ASSERT(result);
	result = ::LineTo(m_hdcCompatible, x(x0), y(y0));
	ASSERT(result);
	
	::SelectObject(m_hdcCompatible, old);
}
void WindowsScreen::rectangle(double x0, double y0, double x1, double y1)
{
	RECT rect = { x(x0)-1, y(y0)-1, x(x1), y(y1) };
	int success = ::FrameRect(m_hdcCompatible, &rect, m_hbrushCurrentColor);
	success;	ASSERT(success);
}
void WindowsScreen::rectangleFill(double x0, double y0, double x1, double y1)
{
	RECT rect = { x(x0), y(y0), x(x1)+1, y(y1)+1 };
	int success = ::FillRect(m_hdcCompatible, &rect, m_hbrushCurrentColor);
	success;	ASSERT(success);
}
void WindowsScreen::circle(double x0, double y0, double r)
{
	double dummy = m_width;
	HGDIOBJ old = ::SelectObject(m_hdcCompatible, m_hpenCurrentColor);
	BOOL success = ::Arc(m_hdcCompatible,
		x(x0-r), y(y0-r),
		x(x0+r), y(y0+r),
		x(x0 + dummy), x(y0 + dummy), x(x0 + dummy), x(y0 + dummy));
	success;	//ASSERT(result);
	::SelectObject(m_hdcCompatible, old);
}
void WindowsScreen::circleFill(double x0, double y0, double r)
{
	HGDIOBJ oldBrush = ::SelectObject(m_hdcCompatible, m_hbrushCurrentColor);
	HGDIOBJ oldPen = ::SelectObject(m_hdcCompatible, m_hpenCurrentColor);
	
	BOOL success = ::Ellipse(m_hdcCompatible, x(x0-r), y(y0-r), x(x0+r), y(y0+r));
	success;	ASSERT(success);
	
	::SelectObject(m_hdcCompatible, oldPen);
	::SelectObject(m_hdcCompatible, oldBrush);
}
void WindowsScreen::arc(double x0, double y0, double r, double startRadians, double endRadians)
{
	double ratio = m_width;
	HGDIOBJ old = ::SelectObject(m_hdcCompatible, m_hpenCurrentColor);
	BOOL success = ::Arc(m_hdcCompatible,
		x(x0-r), y(y0-r),
		x(x0+r), y(y0+r),
		x(x0 + ratio * cos(endRadians)), y(y0 + ratio * sin(endRadians)),
		x(x0 + ratio * cos(startRadians)), y(y0 + ratio * sin(startRadians)));
	success;	//ASSERT(success);
	::SelectObject(m_hdcCompatible, old);
}
void WindowsScreen::write(const char* message)
{
	::TextOut(m_hdcCompatible, 0, 0, message, strlen(message));
}

#endif // } WITH_WINDOW && OS_TYPE == WINDOWS
