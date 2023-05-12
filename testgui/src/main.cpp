#include <simplegui.h>

#include <cmath>
#include <string>
#include <Windows.h>

using namespace simplegui;
int offset;
int lastX = 0, lastY = 0;
std::string text;

class MyPainter : public Painter
{
public:
	virtual void Paint(Window *window, Graphics *g) override
	{
		g->Clear();

		g->SetColor(Color(255, 0, 0));
		g->FillRect(100, 100, 200, 200);




		g->DrawString(100, 100, "Hello World!");

		g->SetColor(Color::BLACK);
		g->FillRect(100, 216, 300, 2);
		g->DrawString(100, 200, text.c_str());
		
		g->SetFillColor(Color::GREEN);
		g->SetLineColor(Color::DARK_GREEN);
		g->FillRect(10, 10, 50, 10);
		
		g->SetFillColor(Color::LIGHT_AQUA);
		g->SetLineColor(Color::AQUA);
		g->DrawEllipse(150 + offset, 20, 40, 100);

		g->SetFillColor(Color::RED);
		g->SetLineColor(Color::DARK_RED);
		g->DrawEllipse(lastX - 10, lastY - 10, 20, 20);
	}
};

class MyMouseListener : public MouseListener
{
public:
	virtual void MouseDown(Window *win, MouseEvent evt) override
	{
		if (evt.button == 1)
		{
			lastX = evt.x;
			lastY = evt.y;
		}
	}
};

class MyKeyListener : public KeyListener
{
public:
	virtual void KeyDown(Window *win, int vk) override
	{
		if (vk == KEY_ESCAPE)
			win->Dispose();
	}

	virtual void KeyTyped(Window *win, unsigned int scancode) override
	{
		switch (scancode)
		{
		case KEY_BACKSPACE:
			if (text.length())
				text = text.substr(0, text.length() - 1);
			break;
		case KEY_RETURN:
			text.push_back('\n');
			break;
		case KEY_TAB:
			text.append("   ");
			break;
		default:
			text.push_back(scancode);
			break;
		}
	}
};

int main(int argc, char *argv[])
{
	MyPainter painter;
	MyMouseListener mListener;
	MyKeyListener kListener;

	Window *window = Window::Create(800, 600, "Hello Window!"); // create the window
	window->SetPainter(&painter); // set the painter
	window->SetMouseListener(&mListener);
	window->SetKeyListener(&kListener);
	window->Show(true); // show the window

	while (!window->IsDisposed())
	{
		double time = (double)GetTickCount64();
		offset = (int)(sin(time / 1000.0) * 50);

		Sleep(20);
		window->Invalidate();
	}

	window->Wait();  // wait for the window to close
	delete window;  // delete the window

	return 0;
}
