#pragma once

#include <cstdint>

#if BUILDING_SIMPLEGUI
#define SIMPLEGUI_API __declspec(dllexport)
#else
#define SIMPLEGUI_API __declspec(dllimport)
#endif

namespace simplegui
{
	class KeyListener;
	class WindowListener;
	class Painter;
	class Graphics;
	class Window;

	//! \brief Listens for key events.
	class SIMPLEGUI_API KeyListener
	{
	public:
		//! \brief Called when a key has been pressed.
		//! 
		//! \param [in] win The window.
		//! \param [in] vk The key.
		virtual void KeyDown(Window *win, int vk);

		//! \brief Called when a character has been typed.
		//! 
		//! \param [in] win The window.
		//! \param [in] scancode The scan code of the key typed.
		virtual void KeyTyped(Window *win, unsigned int scancode);

		//! \brief Called when a key has been released.
		//! 
		//! \param [in] win The window.
		//! \param [in] vk The key.
		virtual void KeyUp(Window *win, int vk);
	};

	//! \brief A mouse event.
	struct MouseEvent
	{
		int x, y; // position
		int button; // mouse button [0,1,2,...]
		int count; // number of button presses
		int mod; // modifier keys held, one of MOD_*
	};

	//! \brief Listens for mouse events.
	class SIMPLEGUI_API MouseListener
	{
	public:
		//! \brief Called when the mouse has moved.
		//! 
		//! \param [in] win The window.
		//! \param [in] x The new x position.
		//! \param [in] y The new y position.
		virtual void MouseMoved(Window *win, int x, int y);

		//! \brief Called when a mouse button has been pressed.
		//! 
		//! \param [in] win The window.
		//! \param [in] evt The event.
		virtual void MouseDown(Window *win, MouseEvent evt);

		//! \brief Called when a mouse has been clicked.
		//! 
		//! \param [in] win The window.
		//! \param [in] evt The event.
		virtual void MouseClick(Window *win, MouseEvent evt);

		//! \brief Called when a mouse has been released.
		//! 
		//! \param [in] win The window.
		//! \param [in] evt The event.
		virtual void MouseUp(Window *win, MouseEvent evt);

		//! \brief Called when the mouse has scrolled.
		//! 
		//! \param [in] win The window.
		//! \param [in] amount The amount of scroll.
		virtual void MouseScroll(Window *win, int amount);
	};

	//! \brief Listens for window events.
	class SIMPLEGUI_API WindowListener
	{
	public:
		//! Called when the window is closing.
		//! 
		//! \param [in] win The window.
		virtual void WindowClosing(Window *win);

		//! \brief Called when the window has gained focus.
		//! 
		//! \param [in] win The window.
		virtual void WindowFocused(Window *win);

		//! \brief Called when the window has lost focus.
		//! 
		//! \param [in] win The window.
		virtual void WindowUnfocused(Window *win);

		//! \brief Called when the window has resized.
		//!
		//! \param [in] win The window.
		//! \brief [in] w The new width.
		//! \brief [in] h The new height.
		virtual void WindowResized(Window *win, int w, int h);

		//! \brief Called when the window has moved.
		//!
		//! \param [in] win The window.
		//! \brief [in] x The new x position.
		//! \brief [in] y The new y position.
		virtual void WindowMoved(Window *win, int x, int y);
	};

	//! \brief Paints a window.
	class SIMPLEGUI_API Painter
	{
	public:
		//! \brief Paint the window.
		//! 
		//! \param [in] win The window.
		//! \param [in] g The window's graphics context.
		virtual void Paint(Window *win, Graphics *g);
	};

	//! \brief Stores a 32-bit ABGR color.
	struct Color
	{
		static constexpr uint32_t BLACK = 0x000000;
		static constexpr uint32_t DARK_GRAY = 0x404040;
		static constexpr uint32_t GRAY = 0x808080;
		static constexpr uint32_t LIGHT_GRAY = 0xbfbfbf;
		static constexpr uint32_t WHITE = 0xffffff;

		static constexpr uint32_t DARK_RED = 0x800000;
		static constexpr uint32_t RED = 0xff0000;
		static constexpr uint32_t LIGHT_RED = 0xffff80;

		static constexpr uint32_t DARK_GREEN = 0x008000;
		static constexpr uint32_t GREEN = 0x00ff00;
		static constexpr uint32_t LIGHT_GREEN = 0x80ff80;

		static constexpr uint32_t DARK_BLUE = 0x000080;
		static constexpr uint32_t BLUE = 0x0000ff;
		static constexpr uint32_t LIGHT_BLUE = 0x8080ff;

		static constexpr uint32_t DARK_YELLOW = 0x808000;
		static constexpr uint32_t YELLOW = 0xffff00;
		static constexpr uint32_t LIGHT_YELLOW = 0xffff80;

		static constexpr uint32_t DARK_MAGENTA = 0x800080;
		static constexpr uint32_t MAGENTA = 0xff00ff;
		static constexpr uint32_t LIGHT_MAGENTA = 0xff80ff;

		static constexpr uint32_t DARK_AQUA = 0x008080;
		static constexpr uint32_t AQUA = 0x00ffff;
		static constexpr uint32_t LIGHT_AQUA = 0x80ffff;

		//! \brief Create a grayscale color. A gray color is defined in that
		//! its R, G, and B fields are all the same.
		//! 
		//! \param [in] amount Amount of grayscale [0, 255].
		//! \param [in,opt] alpha Alpha of color. Default 0.
		//! 
		//! \return Returns a grayscale color with the given amount
		//! and alpha.
		static constexpr Color Gray(int amount, int alpha = 0)
		{
			return Color(amount, amount, amount, alpha);
		}

		// computer is little endian, so order is flipped
		union
		{
			struct { uint8_t r, g, b, a; }; // color components
			uint32_t abgr; // ABGR
		};

		//! \brief Creates black, with 0 as alpha.
		constexpr Color() :
			Color(0, 0, 0, 0) { }

		//! \brief Creates a color with given r, g, and b values and
		//! 0 as alpha.
		//! 
		//! \param [in] r The red component.
		//! \param [in] g The green component.
		//! \param [in] b The blue component.
		constexpr Color(uint8_t r, uint8_t g, uint8_t b) :
			Color(r, g, b, 0) { }

		//! \brief Creates color with given r, g, b, and a values.
		//! 
		//! \param [in] r The red component.
		//! \param [in] g The green component.
		//! \param [in] b The blue component.
		//! \param [in] a The alpha component.
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
			a(a), r(r), g(g), b(b) { }

		//! \brief Creates color from integer code.
		//! 
		//! \param argb [in] The color code.
		constexpr Color(uint32_t argb) :
			abgr(0)
		{
			abgr |= (argb & 0x000000ff) << 16;
			abgr |= (argb & 0x0000ff00);
			abgr |= (argb & 0x00ff0000) >> 16;
			abgr |= (argb & 0xff000000);
		}

		//! \brief Copy a color.
		//! 
		//! \param c [in] The color to copy.
		constexpr Color(const Color &c) :
			abgr(c.abgr) { }

		//! \brief Convert the color to an ARGB color code.
		//! 
		//! \return An ARGB color code.
		constexpr uint32_t ToARGB()
		{
			return 
				((abgr & 0x000000ff) << 16) |
				(abgr & 0x0000ff00) |
				((abgr & 0x00ff0000) >> 16) |
				(abgr & 0xff000000);
		}

		//! \brief Assign this color to another.
		//! 
		//! \param [in] c The color to assign myself to.
		//! 
		//! \return myself.
		constexpr Color &operator=(const Color &c)
		{
			abgr = c.abgr;
			return *this;
		}

		//! \brief Move the color data of another color to myself.
		//! 
		//! \param [in,out] c The color to move.
		//! 
		//! \return myself.
		constexpr Color &operator=(Color &&c) noexcept
		{
			abgr = c.abgr;
			return *this;
		}
	};

	//! \brief Test equality of colors. Colors are equal if all their
	//! components are equal to each other.
	//! 
	//! \param [in] a The first color.
	//! \param [in] b The second color.
	//! 
	//! \return true if the colors are equal and false otherwise.
	constexpr bool operator==(const Color &a, const Color &b)
	{
		return a.abgr == b.abgr;
	}

	//! \brief A graphics context.
	class Graphics
	{
	public:
		//! \brief Draw a rectangle.
		//! 
		//! \param [in] x The x coordinate.
		//! \param [in] y The y coordinate.
		//! \param [in] w The width.
		//! \param [in] h The height,
		virtual void DrawRect(int x, int y, int w, int h) = 0;

		//! \brief Fill a rectangle.
		//! 
		//! \param [in] x The x coordinate.
		//! \param [in] y The y coordinate.
		//! \param [in] w The width.
		//! \param [in] h The height.
		virtual void FillRect(int x, int y, int w, int h) = 0;

		//! \brief Draw an ellipse.
		//! 
		//! \param [in] x The x coordinate.
		//! \param [in] y The y coordinate.
		//! \param [in] w The width.
		//! \param [in] h The height.
		virtual void DrawEllipse(int x, int y, int w, int h) = 0;

		//! \brief Fill an ellipse.
		//! 
		//! \param [in] x The x coordinate.
		//! \param [in] y The y coordinate.
		//! \param [in] w The width.
		//! \param [in] h The height.
		virtual void FillEllipse(int x, int y, int w, int h) = 0;

		//! \brief Draw a line.
		//! 
		//! \param [in] x1 The start x coordinate.
		//! \param [in] y1 The start y coordinate.
		//! \param [in] x2 The end x coordinate.
		//! \param [in] y2 The end y coordinate.
		virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;

		//! \brief Draw text.
		//! 
		//! \param [in] x The x coordinate to draw at.
		//! \param [in] y The y coordinate to draw at.
		//! \param [in] text The text to draw.
		virtual void DrawString(int x, int y, const char *string) = 0;

		//! \brief Set the clipping rectangle.
		//! 
		//! \param [in] x The x position.
		//! \param [in] y The y position.
		//! \param [in] w The width.
		//! \param [in] h The height.
		virtual void SetClipRect(int x, int y, int w, int h) = 0;
		
		//! \brief Set the line color.
		//! 
		//! \param [in] r The red component.
		//! \param [in] g The green component.
		//! \param [in] b The blue component.
		virtual void SetLineColor(int r, int g, int b) = 0;

		//! \brief Set the line color
		//! 
		//! \param [in] color The color.
		virtual void SetLineColor(Color color) = 0;

		//! \brief Set the fill color.
		//!
		//! \param [in] r The red component.
		//! \param [in] g The green component.
		//! \param [in] b The blue component.
		virtual void SetFillColor(int r, int g, int b) = 0;

		//! \brief Set the fill color
		//! 
		//! \param [in] color The color.
		virtual void SetFillColor(Color color) = 0;

		//! \brief Set the line and fill color.
		//!
		//! \param [in] r The red component.
		//! \param [in] g The green component.
		//! \param [in] b The blue component.
		virtual void SetColor(int r, int g, int b) = 0;

		//! \brief Set the line and fill color.
		//!
		//! \param [in] color The color.
		virtual void SetColor(Color color) = 0;

		//! \brief Clear the space.
		virtual void Clear() = 0;

		//! \brief Dispose of internal resources. Does not free this object.
		virtual void Dispose() = 0;
	};

	//! \brief Provides an interface to a Window.
	class SIMPLEGUI_API Window
	{
	public:
		//! \brief Create a window. Destroy the window through the delete operator.
		//! 
		//! \param [in] width The width of the window.
		//! \param [in] height The height of the window.
		//! \param [in,opt] title The title of the window.
		//! 
		//! \return The window.
		static Window *Create(int width, int height, const char *title);
	public:
		Window();
		virtual ~Window();

		//! \brief Set the size of the window.
		//! 
		//! \param [in] w The width.
		//! \param [in] h The height.
		virtual void SetSize(int w, int h) = 0;

		//! \brief Set the position of the window.
		//! 
		//! \param [in] x The x position.
		//! \param [in] y The y position.
		virtual void SetPos(int x, int y) = 0;

		//! \brief Get the size of the window.
		//! 
		//! \param [out,opt] w The width of the window.
		//! \param [out,opt] h The height of the window.
		virtual void GetSize(int *const w, int *const h) = 0;

		//! \brief Get the position of the window.
		//! 
		//! \param [out,opt] x The x position of the window.
		//! \param [out,opt] y The y position of the window.
		virtual void GetPos(int *const x, int *const y) = 0;

		//! \brief Set the title.
		virtual void SetTitle(const char *title) = 0;

		//! \brief Show or hide the window.
		//! 
		//! \param [in] shown Whether to show the window.
		virtual void Show(bool shown) = 0;

		//! \brief Set the background color.
		//! 
		//! \param [in] r The red component.
		//! \param [in] g The green component.
		//! \param [in] b The blue component.
		virtual void SetBackgroundColor(int r, int g, int b) = 0;

		//! \brief Set the background color.
		//! 
		//! \param [in] color The color.
		virtual void SetBackgroundColor(Color color) = 0;

		//! \brief Force paints the window. Many calls to this can significantly
		//! slow down the system. Prefer using Invalidate().
		virtual void Paint() = 0;
		
		//! \brief Force repaints the window. Many calls to this can significantly
		//! slow down the system. Prefer using Invalidate().
		virtual void Repaint() = 0;

		//! \brief Invalidate the window, which will trigger a repaint in the future.
		virtual void Invalidate() = 0;

		//! \brief Validate the window.
		virtual void Validate() = 0;

		//! \brief Revalidate the window.
		virtual void Revalidate() = 0;

		//! \brief Wait for the window to close.
		virtual void Wait() = 0;

		//! \brief Release the window.
		virtual void Dispose() = 0;

		//! \brief Returns whether the window has been disposed.
		//! 
		//! \return true if the window has been disposed and false otherwise.
		virtual bool IsDisposed() = 0;

		//! \brief Set or remove the key listener.
		//! 
		//! \param [in,opt] kl The key listener. The window does not own this.
		virtual void SetKeyListener(KeyListener *kl) = 0;

		//! \brief Set or remove the mouse listener.
		//! 
		//! \param [in,opt] ml The mouse listener. The window does not own this.
		virtual void SetMouseListener(MouseListener *ml) = 0;

		//! \brief Set or remove the window listener.
		//! 
		//! \param [in,opt] wl The window listener. The window does not own this.
		virtual void SetWindowListener(WindowListener *wl) = 0;

		//! \brief Set or remove the window's painter.
		//! 
		//! \param [in,opt] p The new painter. The window does not own this.
		virtual void SetPainter(Painter *p) = 0;

		//! \brief Get the state of a key in this window.
		//! 
		//! \param [in] vk The virtual key code.
		//! 
		//! \return true if the key is pressed and false otherwise. If the key
		//! does not exist, false is returned.
		virtual bool GetAsyncKey(int vk) = 0;

		//! \brief Get the state of multiple keys in this window.
		//! 
		//! \param [in] vks An array of virtual key codes to get the state of.
		//! \param [out] states An array of bools whose respective elements will
		//! give the state of the keys in vks. If the respective key does not exist,
		//! the element will be set to false.
		//! \param [in] count The number of elements in vks and states.
		virtual void GetAsyncKeys(const int *vks, bool *const states, int count) = 0;

		//! \brief Get the state of a mouse button in this window.
		//! 
		//! \param [in] mb The mouse button (1, 2, 3, ...).
		//! 
		//! \return true if the mouse button is pressed and false otherwise. If
		//! the mouse button does not exist, false is returned.
		virtual bool GetAsyncMouseButton(int mb) = 0;

		//! \brief Get the state of multiple mouse buttons in this window.
		//! 
		//! \param [in] mbs An array of mouse buttons to get the state of.
		//! \param [out] states An array of bools whose respective elements will
		//! give the state of the mouse button in mbs. If the respective mouse
		//! button does not exist, the element will be set to false.
		//! \param [in] count The number of elements in mbs and states.
		virtual void GetAsyncMouseButtons(const int *mbs, bool *const states, int count) = 0;

		//! \brief Get the position of the mouse in this window.
		//! 
		//! \param [out,opt] x The x position of the mouse.
		//! \param [out,opt] y The y position of the mouse.
		virtual void GetAsyncMousePosition(int *const x, int *const y) = 0;

		//! \brief Create a child window.
		//! 
		//! \param [in] width The width of the window.
		//! \param [in] height The height of the window.
		//! \param [in,opt] title The title of the window.
		//! 
		//! \return The new window.
		virtual Window *CreateChild(int width, int height, const char *title) = 0;
	};

	/* modifier keys */
	enum
	{
		MOD_LSHIFT = 0x1,
		MOD_RSHIFT = 0x2,
		MOD_SHIFT = MOD_LSHIFT | MOD_RSHIFT,

		MOD_LALT = 0x4,
		MOD_RALT = 0x8,
		MOD_ALT = MOD_LALT | MOD_RALT,

		MOD_LWIN = 0x10,
		MOD_RWIN = 0x20,
		MOD_WIN = MOD_LWIN | MOD_RWIN
	};

	/* virtual key codes */
	enum
	{
		KEY_BACKSPACE = 0x8,
		KEY_TAB,

		KEY_CLEAR = 0x0c,
		KEY_RETURN,

		KEY_SHIFT = 0x10,
		KEY_CONTROL,
		KEY_MENU,
		KEY_PAUSE,
		KEY_CAPITAL,

		KEY_ESCAPE = 0x1b,

		KEY_CONVERT = 0x1c,
		KEY_NONCONVERT,
		KEY_ACCEPT,
		KEY_MODECHANGE,

		KEY_SPACE = 0x20,
		KEY_PRIOR,
		KEY_NEXT,
		KEY_END,
		KEY_HOME,
		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,
		KEY_SELECT,
		KEY_PRINT,
		KEY_EXECUTE,
		KEY_SNAPSHOT,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HELP,

		KEY_0 = '0',
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,

		KEY_A = 'A',
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,

		KEY_LWIN = 0x5b,
		KEY_RWIN,
		KEY_APPS,

		KEY_SLEEP = 0x5f,

		KEY_NP0 = 0x60,
		KEY_NP1,
		KEY_NP2,
		KEY_NP3,
		KEY_NP4,
		KEY_NP5,
		KEY_NP6,
		KEY_NP7,
		KEY_NP8,
		KEY_NP9,
		KEY_MULTIPLY,
		KEY_ADD,
		KEY_SEPARATOR,
		KEY_SUBTRACT,
		KEY_DECIMAL,
		KEY_DIVICE,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_F16,
		KEY_F17,
		KEY_F18,
		KEY_F19,
		KEY_F20,
		KEY_F21,
		KEY_F22,
		KEY_F23,
		KEY_F24,

		KEY_NAV_VIEW = 0x88,
		KEY_NAV_MENU,
		KEY_NAV_UP,
		KEY_NAV_DOWN,
		KEY_NAV_LEFT,
		KEY_NAV_RIGHT,
		KEY_NAV_ACCEPT,
		KEY_NAV_CANCEL,

		KEY_NUMLOCK = 0x90,
		KEY_SCROLL,

		KEY_NP_EQUAL = 0x92,

		KEY_LSHIFT = 0xa0,
		KEY_RSHIFT,
		KEY_LCONTROL,
		KEY_RCONTROL,
		KEY_LMENU,
		KEY_RMENU,

		KEY_SEMI = 0xba,
		KEY_PLUS,
		KEY_COMMA,
		KEY_MINUS,
		KEY_PERIOD,
		KEY_FSLASH,
		KEY_GRAVE,

		KEY_RBRACKET = 0xdb,
		KEY_BSLASH,
		KEY_LBRACKET,
		KEY_QUOTE
	};
}