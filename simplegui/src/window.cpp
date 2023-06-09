#include <simplegui.h>

#include <Windows.h>
#include <windowsx.h>

using namespace simplegui;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

class Win32Graphics : public Graphics
{
public:
	PAINTSTRUCT ps;
	HWND hwnd;

	Win32Graphics(HWND hwnd) :
		hwnd(hwnd)
	{
		BeginPaint(hwnd, &ps);
	}

	virtual ~Win32Graphics()
	{
		Dispose();
	}

	virtual void DrawRect(int x, int y, int w, int h) override
	{
		if (!hwnd) return;

		MoveToEx(ps.hdc, x, y, NULL);
		LineTo(ps.hdc, x + w, h);

		MoveToEx(ps.hdc, x + w, y, NULL);
		LineTo(ps.hdc, x + w, y + h);

		MoveToEx(ps.hdc, x + w, y + h, NULL);
		LineTo(ps.hdc, x, y + h);

		MoveToEx(ps.hdc, x, y + h, NULL);
		LineTo(ps.hdc, x, y);

	}

	virtual void FillRect(int x, int y, int w, int h) override
	{
		if (!hwnd) return;
		Rectangle(ps.hdc, x, y, x + w, y + h);
	}

	virtual void DrawEllipse(int x, int y, int w, int h) override
	{
		if (!hwnd) return;
		Ellipse(ps.hdc, x, y, x + w, y + h);
	}

	virtual void FillEllipse(int x, int y, int w, int h) override
	{
		if (!hwnd) return;
		Ellipse(ps.hdc, x, y, x + h, y + h);
	}

	virtual void DrawLine(int x1, int y1, int x2, int y2) override
	{
		if (!hwnd) return;

		MoveToEx(ps.hdc, x1, y1, NULL);
		LineTo(ps.hdc, x2, y2);
	}

	virtual void DrawString(int x, int y, const char *string) override
	{
		if (!hwnd) return;

		int chCount = (int)strlen(string);
		SIZE sizl;

		/* get extents of text */
		GetTextExtentPoint32A(
			ps.hdc,
			string,
			chCount,
			&sizl);

		RECT rc;
		rc.left = x;
		rc.top = y;
		rc.right = x + sizl.cx;
		rc.bottom = y + sizl.cy;

		/* draw text */
		DrawTextA(
			ps.hdc,
			string,
			chCount,
			&rc,
			DT_LEFT);
	}

	virtual void SetClipRect(int x, int y, int w, int h) override
	{
		if (!hwnd) return;
	}

	virtual void SetLineColor(int r, int g, int b) override
	{
		SetLineColor(Color(r, g, b));
	}

	virtual void SetLineColor(Color color) override
	{
		if (!hwnd) return;
		SelectObject(ps.hdc, GetStockObject(DC_PEN));
		SetDCPenColor(ps.hdc, color.abgr);
	}

	virtual void SetFillColor(int r, int g, int b) override
	{
		SetFillColor(Color(r, g, b));
	}

	virtual void SetFillColor(Color color) override
	{
		if (!hwnd) return;
		SelectObject(ps.hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(ps.hdc, color.abgr);
	}

	virtual void SetColor(int r, int g, int b) override
	{
		SetColor(Color(r, g, b));
	}
		
	virtual void SetColor(Color color) override
	{
		SetLineColor(color);
		SetFillColor(color);
	}

	virtual void Clear() override
	{
		if (!hwnd) return;

		RECT rc;
		GetClientRect(hwnd, &rc);
		::FillRect(ps.hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
	}

	virtual void Dispose() override
	{
		if (hwnd)
		{
			EndPaint(hwnd, &ps);
			hwnd = NULL;
		}
	}
};

//! \brief Win32 API window
class Win32Window : public Window
{
public:
	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//! \brief Setup the window
	//! 
	//! \param [in] win The window to setup.
	//! 
	//! \return The HWND.
	static HWND SetupWindow(Win32Window *win)
	{
		EnterCriticalSection(&win->cs);

		WNDCLASSA wc = { 0 };
		wc.lpfnWndProc = &WindowProc;
		wc.lpszClassName = "Window";
		wc.hInstance = GetModuleHandleA(NULL);
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		RegisterClassA(&wc);

		HWND parent = win->parent ? win->parent->hwnd : NULL;
		win->hwnd = CreateWindowExA(
			0,
			"Window",
			"",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			parent,
			NULL,
			NULL,
			win);

		SetWindowLongPtrA(win->hwnd, GWLP_USERDATA, (LONG_PTR)win);

		LeaveCriticalSection(&win->cs);

		WakeAllConditionVariable(&win->cv);
		return win->hwnd;
	}

	//! \brief The main event loop.
	//! 
	//! \param [in] win The window.
	static void EventLoop(Win32Window *win)
	{
		BOOL bRet;
		MSG msg;

		while ((bRet = GetMessageA(&msg, NULL, 0, 0)) != 0)
		{
			if (bRet == -1)
				MessageBoxA(win->hwnd, "Win32Window::EventLoop(): Unresolved Error", "Error", MB_ICONERROR);

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	//! \brief Entry point for the window worker.
	//! 
	//! \param [in] win The window
	//! 
	//! \return 0
	static DWORD CALLBACK WindowWorker(Win32Window *win)
	{

		HWND hwnd;

		hwnd = SetupWindow(win);
		EventLoop(win);
		DestroyWindow(hwnd);

		return 0;
	}
	
	HWND hwnd;
	HANDLE hThread;
	CRITICAL_SECTION cs;
	CONDITION_VARIABLE cv;
	int bgcolor;

	KeyListener *kl;
	MouseListener *ml;
	WindowListener *wl;
	Painter *p;

	Win32Window *parent;

	static constexpr int nKeys = 256;
	bool keys[nKeys];

	static constexpr int nMbuttons = 16;
	bool mbuttons[nMbuttons];

	bool painting;

	int ModifierKeys()
	{
		int mods = 0;

		if (keys[KEY_LSHIFT]) mods |= MOD_LSHIFT;
		if (keys[KEY_RSHIFT]) mods |= MOD_RSHIFT;

		if (keys[KEY_LMENU]) mods |= MOD_LALT;
		if (keys[KEY_RMENU]) mods |= MOD_RALT;

		if (keys[KEY_LWIN]) mods |= MOD_LWIN;
		if (keys[KEY_RWIN]) mods |= MOD_RWIN;
		
		return mods;
	}

	Win32Window(int width, int height, const char *title, Win32Window *parent) :
		hwnd(NULL), hThread(NULL),
		kl(0), ml(0), wl(0), p(0),
		bgcolor(RGB(200, 200, 200)),
		painting(false), parent(parent)
	{
		/* keys and mouse buttons not pressed initially */
		ZeroMemory(keys, sizeof(keys));
		ZeroMemory(mbuttons, sizeof(mbuttons));

		/* create synchronization primitives */
		InitializeCriticalSection(&cs);
		InitializeConditionVariable(&cv);

		/* create worker thread */
		hThread = CreateThread(
			NULL,
			0,
			(LPTHREAD_START_ROUTINE)&WindowWorker,
			this,
			0,
			NULL);

		/* wait for window creation to complete */
		EnterCriticalSection(&cs);
		while (!hwnd)
			SleepConditionVariableCS(&cv, &cs, INFINITE);
		LeaveCriticalSection(&cs);

		/* set size and title */
		SetSize(width, height);
		SetTitle(title);
	}

	virtual ~Win32Window()
	{
		Dispose();

		/* wait for thread to complete */
		if (hThread)
		{
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}

		DeleteCriticalSection(&cs);
	}

	virtual void SetSize(int w, int h) override
	{
		RECT rc;

		EnterCriticalSection(&cs);

		if (hwnd)
		{
			GetWindowRect(hwnd, &rc);
			SetWindowPos(hwnd, NULL, rc.left, rc.top, w, h, 0);
		}

		LeaveCriticalSection(&cs);
	}

	virtual void SetPos(int x, int y) override
	{
		RECT rc;

		EnterCriticalSection(&cs);

		if (hwnd)
		{
			GetWindowRect(hwnd, &rc);
			SetWindowPos(hwnd, NULL, x, y, rc.right - rc.left, rc.bottom - rc.top, 0);
		}

		LeaveCriticalSection(&cs);
	}

	virtual void GetSize(int *const w, int *const h) override
	{
		RECT rc;

		EnterCriticalSection(&cs);

		if (hwnd)
		{
			GetWindowRect(hwnd, &rc);
			if (w) *w = rc.right - rc.left;
			if (h) *h = rc.bottom - rc.top;
		}

		LeaveCriticalSection(&cs);
	}

	virtual void GetPos(int *const x, int *const y) override
	{
		RECT rc;

		EnterCriticalSection(&cs);

		if (hwnd)
		{
			GetWindowRect(hwnd, &rc);
			if (x) *x = rc.left;
			if (y) *y = rc.top;
		}

		LeaveCriticalSection(&cs);
	}

	virtual void SetTitle(const char *title) override
	{
		EnterCriticalSection(&cs);

		if (hwnd)
			SetWindowTextA(hwnd, title);

		LeaveCriticalSection(&cs);
	}

	virtual void Show(bool shown) override
	{
		EnterCriticalSection(&cs);

		if (hwnd)
			ShowWindow(hwnd, shown ? SW_SHOW : SW_HIDE);

		LeaveCriticalSection(&cs);
	}

	virtual void SetBackgroundColor(int r, int g, int b) override
	{
		EnterCriticalSection(&cs);
		bgcolor = RGB(r, g, b);
		LeaveCriticalSection(&cs);
	}

	virtual void SetBackgroundColor(Color color) override
	{
		EnterCriticalSection(&cs);
		bgcolor = color.abgr;
		LeaveCriticalSection(&cs);
	}

	virtual void Paint() override
	{
		EnterCriticalSection(&cs);

		if (hwnd)
		{
			PostMessageA(
				hwnd,
				WM_PAINT,
				0,
				0);
		}

		LeaveCriticalSection(&cs);
	}

	virtual void Repaint() override
	{
		Paint();
	}

	virtual void Invalidate() override
	{
		EnterCriticalSection(&cs);

		InvalidateRect(hwnd, NULL, TRUE);

		LeaveCriticalSection(&cs);
	}

	virtual void Validate() override { }

	virtual void Revalidate() override
	{
		EnterCriticalSection(&cs);

		Invalidate();
		Validate();
	
		LeaveCriticalSection(&cs);
	}

	virtual void Wait() override
	{
		WaitForSingleObject(hThread, INFINITE);
	}

	virtual void Dispose() override
	{
		EnterCriticalSection(&cs);

		if (hwnd)
		{
			PostMessageA(
				hwnd,
				WM_QUIT,
				0,
				0);
			hwnd = NULL;
		}
		
		LeaveCriticalSection(&cs);
	}

	virtual bool IsDisposed() override
	{
		EnterCriticalSection(&cs);
		bool result = !hwnd;
		LeaveCriticalSection(&cs);
		return result;
	}

	virtual void SetKeyListener(KeyListener *kl) override
	{
		EnterCriticalSection(&cs);
		this->kl = kl;
		LeaveCriticalSection(&cs);
	}

	virtual void SetMouseListener(MouseListener *ml) override
	{
		EnterCriticalSection(&cs);
		this->ml = ml;
		LeaveCriticalSection(&cs);
	}

	virtual void SetWindowListener(WindowListener *wl) override
	{
		EnterCriticalSection(&cs);
		this->wl = wl;
		LeaveCriticalSection(&cs);
	}

	virtual void SetPainter(Painter *p) override
	{
		EnterCriticalSection(&cs);
		this->p = p;
		LeaveCriticalSection(&cs);
	}

	virtual bool GetAsyncKey(int vk) override
	{
		EnterCriticalSection(&cs);
		bool r = false;
		if (vk >= 0 && vk < nKeys)
			r = keys[vk];
		LeaveCriticalSection(&cs);
		return r;
	}

	virtual void GetAsyncKeys(const int *vks, bool *const states, int count) override
	{
		EnterCriticalSection(&cs);
		for (int i = 0; i < count; i++)
		{
			int vk = vks[i];
			if (vk < 1 || vk > nKeys)
				continue;
			states[i] = keys[vk];
		}
		LeaveCriticalSection(&cs);
	}

	virtual bool GetAsyncMouseButton(int mb) override
	{
		EnterCriticalSection(&cs);
		bool r = false;
		if (mb >= 1 && mb <= nMbuttons)
			r = mbuttons[mb - 1];
		LeaveCriticalSection(&cs);
		return r;
	}

	virtual void GetAsyncMouseButtons(const int *mbs, bool *const states, int count) override
	{
		EnterCriticalSection(&cs);
		for (int i = 0; i < count; i++)
		{
			int mb = mbs[i];
			if (mb < 1 || mb > nMbuttons)
				continue;
			states[i] = mbuttons[mb - 1];
		}
		LeaveCriticalSection(&cs);
	}

	virtual void GetAsyncMousePosition(int *const x, int *const y) override
	{
		POINT pPos;
		GetCursorPos(&pPos);
		ScreenToClient(hwnd, &pPos);
		if (*x) *x = pPos.x;
		if (*y) *y = pPos.y;
	}

	virtual Window *CreateChild(int width, int height, const char *title) override
	{
		return new Win32Window(width, height, title, this);
	}
};

Window *simplegui::Window::Create(int width, int height, const char *title)
{
	return new Win32Window(width, height, title, nullptr);
}

simplegui::Window::Window() { }
simplegui::Window::~Window() { }

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Win32Window *win;
	MouseEvent evt;

	win = (Win32Window *)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

	switch (Msg)
	{
	case WM_NCCREATE:
	case WM_CREATE:
		return DefWindowProcA(hWnd, Msg, wParam, lParam);
	case WM_QUIT:
		return 0;

	case WM_PAINT: {
		if (!win->p)
			break;

		Win32Graphics g(hWnd);
		win->p->Paint(win, &g);
		return 0;
	}

	/* window events */

	case WM_CLOSE: // closing
		if (win->wl)
			win->wl->WindowClosing(win);
		else
			win->Dispose();
		return 0;
	
	case WM_SETFOCUS: // gained focus
		if (win->wl)
			win->wl->WindowFocused(win);
		return 0;
	case WM_KILLFOCUS: // lost focus
		if (win->wl)
			win->wl->WindowUnfocused(win);
		return 0;
	case WM_SIZE: // resized
		if (win->wl)
			win->wl->WindowResized(win, LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_MOVE: // moved
		if (win->wl)
			win->wl->WindowMoved(win, LOWORD(lParam), HIWORD(lParam));
		return 0;

	/* keyboard events */
	case WM_KEYDOWN: { // key pressed
		bool old = win->keys[wParam];
		win->keys[wParam] = true;
		if (!old && win->kl)
			win->kl->KeyDown(win, (int)wParam);
		return 0;
	}
	case WM_CHAR: // key typed
		if (win->kl)
			win->kl->KeyTyped(win, (unsigned int)wParam);
		return 0;
	case WM_KEYUP: {// key released
		bool old = win->keys[wParam];
		win->keys[wParam] = false;
		if (old && win->kl)
			win->kl->KeyUp(win, (int)wParam);
		return 0;
	}
	
	/* ignore system keys */
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return 0;

	/* mouse events */
	case WM_MOUSEMOVE:
		if (win->ml)
			win->ml->MouseMoved(win, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		evt.button = 1;
		goto mb_down_generic;
	case WM_RBUTTONDOWN:
		evt.button = 2;
		goto mb_down_generic;
	case WM_MBUTTONDOWN:
		evt.button = 3;
	mb_down_generic:
		win->mbuttons[evt.button - 1] = true;
		if (win->ml)
		{
			evt.count = 0;
			evt.mod = win->ModifierKeys();
			evt.x = GET_X_LPARAM(lParam);
			evt.y = GET_Y_LPARAM(lParam);
			win->ml->MouseDown(win, evt);
		}
		return 0;
	// TODO: clicks
	
	case WM_LBUTTONUP:
		evt.button = 1;
		goto mb_up_generic;
	case WM_RBUTTONUP:
		evt.button = 2;
		goto mb_up_generic;
	case WM_MBUTTONUP:
		evt.button = 3;
	mb_up_generic:
		win->mbuttons[evt.button - 1] = false;
		if (win->ml)
		{
			evt.count = 0;
			evt.mod = 0;
			evt.x = GET_X_LPARAM(lParam);
			evt.y = GET_Y_LPARAM(lParam);
			win->ml->MouseUp(win, evt);
		}
		return 0;
	case WM_MOUSEWHEEL: {
		if (win->ml)
		{
			evt.count = 0;
			evt.mod = 0;
			evt.mod = win->ModifierKeys();
			evt.x = GET_X_LPARAM(lParam);
			evt.y = GET_Y_LPARAM(lParam);
			win->ml->MouseScroll(win, WHEEL_DELTA * GET_WHEEL_DELTA_WPARAM(wParam));
		}
		return 0;
	}
	}

	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}