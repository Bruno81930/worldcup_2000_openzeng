// WindowsScreen.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(WINDOWSSCREEN_HXX__INCLUDED) // {
#define WINDOWSSCREEN_HXX__INCLUDED

#if WITH_WINDOW && OS_TYPE == WINDOWS // {

#include "Screen.hxx"

/////////////////////////////////////////////////////////////////////////////
// WindowsScreen

class WindowsScreen : public Screen
{
private:
	HDC m_hdcCompatible;
	HGDIOBJ m_hbitmapOriginal;
	
	HBRUSH m_hbrushCurrentColor;
	HPEN m_hpenCurrentColor;
	
	int m_screenWidth, m_screenHeight;
	double m_width, m_height;
	double m_ratioX, m_ratioY;
	double m_baseX, m_baseY;
	
	int x(double v);
	int y(double v);
	int w(double v);
	int h(double v);
	
protected:
	
public:
	virtual ~WindowsScreen();
	WindowsScreen();
	
	void set(int width, int height);
	
	void drawTo(HDC hdc);
	
	virtual int width() const;
	virtual int height() const;
	
	virtual void fillBackground();
	virtual void color(int r, int g, int b);
	virtual void line(double x0, double y0, double x1, double y1);
	virtual void rectangle(double x0, double y0, double x1, double y1);
	virtual void rectangleFill(double x0, double y0, double x1, double y1);
	virtual void circle(double x0, double y0, double r);
	virtual void circleFill(double x0, double y0, double r);
	virtual void arc(double x0, double y0, double r, double startRadians, double endRadians);
	virtual void write(const char* message);
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "WindowsScreen.inl"
#endif

#endif // } WITH_WINDOW && OS_TYPE == WINDOWS
#endif // } !defined(WINDOWSSCREEN_HXX__INCLUDED)
