// WindowsWindow.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#if WITH_WINDOW && OS_TYPE == WINDOWS // {
#include "WindowsWindow.hxx"

#include "WindowsScreen.hxx"
#include "resource.h"

#ifndef NDEBUG
#  include "WindowsWindow.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// WindowsWindow

WindowsWindow::~WindowsWindow()
{
	if(m_hWnd) {
		BOOL b = ::DestroyWindow(m_hWnd);
		b;	ASSERT(b);
	}
	delete m_screen;
}

WindowsWindow::WindowsWindow(const char* title)
{
	m_hWnd = NULL;
	m_screen = NULL;
	
	static const char className[] = "SoccerClientWindow";
	HINSTANCE theInstance = ::GetModuleHandle(NULL);
	
	WNDCLASSEX wndClass;
	::ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpfnWndProc = &windowProc;
	wndClass.cbWndExtra = sizeof(this);
	wndClass.hInstance = theInstance;
	wndClass.hIcon = ::LoadIcon(theInstance, MAKEINTRESOURCE(IDI_ICON));
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszClassName = className;
	ATOM atom = ::RegisterClassEx(&wndClass);
	atom;	ASSERT(atom);
	
	m_hWnd = ::CreateWindowEx(WS_EX_APPWINDOW,
		className, title,
		WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, theInstance, (LPVOID)this);
	ASSERT(m_hWnd);
}

void WindowsWindow::go()
{
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void WindowsWindow::updateScreen()
{
	if(m_hWnd) {
		RECT rect;
		BOOL result = ::GetClientRect(m_hWnd, &rect);
		//ASSERT(result);
		if(result)
			result = ::InvalidateRect(m_hWnd, &rect, false);
		//ASSERT(result);
	}
}

LRESULT CALLBACK WindowsWindow::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowsWindow* This;
	if(uMsg == WM_CREATE) {
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		This = (WindowsWindow*)lpcs->lpCreateParams;
		::SetWindowLong(hwnd, 0, (DWORD)(LPVOID)This);
	} else {
		This = (WindowsWindow*)(LPVOID)::GetWindowLong(hwnd, 0);
		if(This == 0 || This->m_hWnd == 0)
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return This->wndProc(uMsg, wParam, lParam);
}

LRESULT WindowsWindow::wndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool continuous = false;
	LRESULT result = 0;
	switch(uMsg) {
	case WM_CREATE:
		result = wmCreate(continuous) ? 0 : -1;
		break;
	case WM_DESTROY:
		wmDestroy(continuous);
		m_hWnd = 0;
		break;
	case WM_PAINT:
		wmPaint(continuous);
		break;
	case WM_SIZE:
		wmSize(LOWORD(lParam), HIWORD(lParam), continuous);
		break;
	case WM_KEYDOWN:
		wmKeyDown((int)wParam, lParam, continuous);
		break;
	default:
		continuous = true;
	}
	if(!continuous)
		return result;
	
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

bool WindowsWindow::wmCreate(bool& /*continuous*/)
{
	return true;
}
void WindowsWindow::wmDestroy(bool& /*continuous*/)
{
	::PostQuitMessage(0);
}
void WindowsWindow::wmPaint(bool& /*continuous*/)
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(m_hWnd, &ps); 
	{
		if(!m_screen) {
			m_screen = new WindowsScreen();
			
			RECT rect;
			BOOL b = ::GetClientRect(m_hWnd, &rect);
			b;	ASSERT(b);
			m_screen->set(rect.right, rect.bottom);
		}
		
#if 1
		this->paint(*m_screen);
		m_screen->drawTo(hdc);
#else
		int width = m_screen->width();
		int height = m_screen->height();
		
		HDC hcdCompatible = ::CreateCompatibleDC(NULL);
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, width, height);
		HANDLE oldBitmap = ::SelectObject(hcdCompatible, hBitmap);
		{
			m_screen->set(hcdCompatible);
			this->paintScreen(*m_screen);
			::BitBlt(hdc, 0, 0, width, height, hcdCompatible, 0, 0, SRCCOPY);
		}
		::SelectObject(hcdCompatible, oldBitmap);
		::DeleteObject(hBitmap);
		::DeleteDC(hcdCompatible);
#endif
	}
	::EndPaint(m_hWnd, &ps);
}

void WindowsWindow::wmSize(int width, int height, bool& /*continuous*/)
{
	m_screen->set(width, height);
	
	RECT rect = { 0, 0, width, height };
	::InvalidateRect(m_hWnd, &rect, false);
}
void WindowsWindow::wmKeyDown(int nVirtKey, int /*lKeyData*/, bool& continuous)
{
	switch(nVirtKey) {
	case VK_LEFT:
		keyPushed(KEY_LEFT);
		break;
	case VK_RIGHT:
		keyPushed(KEY_RIGHT);
		break;
	case VK_UP:
		keyPushed(KEY_UP);
		break;
	case VK_DOWN:
		keyPushed(KEY_DOWN);
		break;
	case VK_SPACE:
		keyPushed(KEY_SPACE);
		break;
	default:
		continuous = true;
	}
}

#endif // } WITH_WINDOW && OS_TYPE == WINDOWS // {
