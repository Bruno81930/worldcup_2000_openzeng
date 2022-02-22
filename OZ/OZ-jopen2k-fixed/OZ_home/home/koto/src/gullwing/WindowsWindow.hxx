// WindowsWindow.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(WINDOWSWINDOW_HXX__INCLUDED) // {
#define WINDOWSWINDOW_HXX__INCLUDED

#if WITH_WINDOW && OS_TYPE == WINDOWS // {

#include "Window.hxx"
class WindowsScreen;

/////////////////////////////////////////////////////////////////////////////
// WindowsWindow

class WindowsWindow : public Window
{
private:
	HWND m_hWnd;
	WindowsScreen* m_screen;
	
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual LRESULT wndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool wmCreate(bool& continuous);
	virtual void wmDestroy(bool& continuous);
	virtual void wmPaint(bool& continuous);
	virtual void wmSize(int width, int height, bool& continuous);
	virtual void wmKeyDown(int nVirtKey, int lKeyData, bool& continuous);
	
public:
	virtual ~WindowsWindow();
	WindowsWindow(const char* title);
	
	virtual void go();
	virtual void updateScreen();	// Window のメインスレッド以外のスレッドからアクセスされる。
};

typedef WindowsWindow WindowImplemented;

/////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#  include "WindowsWindow.inl"
#endif

#endif // } WITH_WINDOW && OS_TYPE == WINDOWS
#endif // } !defined(WINDOWSWINDOW_HXX__INCLUDED)
