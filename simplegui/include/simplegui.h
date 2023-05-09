#pragma once

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
		int mod; // modifier keys held
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

	//! \brief A color
	enum class Color
	{
		BLACK = 0x000000,
		DARK_GRAY = 0x404040,
		GRAY = 0x808080,
		LIGHT_GRAY = 0xbfbfbf,
		WHITE = 0xffffff,

		DARK_RED = 0x000080,
		RED = 0x0000ff,
		LIGHT_RED = 0x80ffff,

		DARK_GREEN = 0x008000,
		GREEN = 0x00ff00,
		LIGHT_GREEN = 0x80ff80,

		DARK_BLUE = 0x800000,
		BLUE = 0xff0000,
		LIGHT_BLUE = 0xff8080,

		DARK_YELLOW = 0x008080,
		YELLOW = 0x00ffff,
		LIGHT_YELLOW = 0x80ffff,

		DARK_MAGENTA = 0x800080,
		MAGENTA = 0xff00ff,
		LIGHT_MAGENTA = 0xff80ff,

		DARK_AQUA = 0x808000,
		AQUA = 0xffff00,
		LIGHT_AQUA = 0xffff80
	};

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

		//! \brief Paint the window and wait for it to finish.
		virtual void PaintAndWait() = 0;

		//! \brief Paint the window.
		virtual void Paint() = 0;
		
		//! \brief Repaint the window.
		virtual void Repaint() = 0;

		//! \brief Invalidate the window.
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
	};

	//! \brief Create a window.
	//! 
	//! \param [in] width The width of the window.
	//! \param [in] height The height of the window.
	//! \param [in,opt] title The title of the window.
	//! 
	//! \return The window.
	SIMPLEGUI_API Window *NewWindow(int width, int height, const char *title);
}