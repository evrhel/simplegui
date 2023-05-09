#include <simplegui.h>

#include <cmath>
#include <Windows.h>

using namespace simplegui;
int offset;
int lastX = 0, lastY = 0;

class MyPainter : public Painter
{
public:
	virtual void Paint(Window *window, Graphics *g) override
	{
		g->Clear();

		g->DrawString(100, 100, "Hello World!");
		
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

class MyListener : public MouseListener
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

int main(int argc, char *argv[])
{
	MyPainter painter;
	MyListener listener;

	Window *window = NewWindow(800, 600, "Hello Window!"); // create the window
	window->SetPainter(&painter); // set the painter
	window->SetMouseListener(&listener);
	window->Show(true); // show the window

	while (!window->IsDisposed())
	{
		double time = GetTickCount64();
		offset = (int)(sin(time / 1000.0) * 50);

		Sleep(20);
		window->Invalidate();
	}

	window->Wait();  // wait for the window to close
	delete window;  // delete the window

	return 0;
}
