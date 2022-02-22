// Screen.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SCREEN_HXX__INCLUDED) // {
#define SCREEN_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Screen

/** システムの状態を表示するスクリーン */
class Screen
{
public:
	virtual ~Screen();
	Screen();
	
	int colorBackground() { return 0x008000; }
	int colorDark() { return 0x006000; }
	int colorLine() { return 0xFFFFFF; }
	int colorFlag() { return 0xC0C0C0; }
	int colorBall() { return 0xFF80FF; }
	int colorBallAnticipated() { return 0x800080; }
	int colorGreen() { return 0x00FF00; }
	int colorYellow() { return 0xFFFF00; }
	int colorRed() { return 0xFF8080; }
	int colorCorrect() { return 0xFF80FF; }
	int colorThinking() { return 0xFFFFFF; }
	
	virtual int width() const = 0;
	virtual int height() const = 0;
	
	virtual void fillBackground() = 0;
	virtual void color(int r, int g, int b) = 0;
	void color(int rgb);
	virtual void line(double x0, double y0, double x1, double y1) = 0;
	virtual void rectangle(double x0, double y0, double x1, double y1);
	virtual void rectangleFill(double x0, double y0, double x1, double y1) = 0;
	virtual void circle(double x0, double y0, double r) = 0;
	virtual void circleFill(double x0, double y0, double r) = 0;
	virtual void arc(double x0, double y0, double r, double startRadians, double endRadians) = 0;
	virtual void write(const char* message) = 0;
	
	void paintField();
};

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "Screen.inl"
#endif

#endif // } !defined(SCREEN_HXX__INCLUDED)
