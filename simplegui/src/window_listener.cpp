#include <simplegui.h>

void simplegui::WindowListener::WindowClosing(Window *win)
{
	win->Dispose();
}

void simplegui::WindowListener::WindowFocused(Window *win) { }
void simplegui::WindowListener::WindowUnfocused(Window *win) { }
void simplegui::WindowListener::WindowResized(Window *win, int w, int h) { }
void simplegui::WindowListener::WindowMoved(Window *win, int x, int y) { }
