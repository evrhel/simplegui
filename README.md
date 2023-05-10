# simplegui

A simple C++ abstraction of Window management and the Graphics Device
Interface (GDI) for Windows.

Everything is defined in the `simplegui.h` header file. `simplegui` is
the namespace for all classes and functions.

The library builds to a DLL. To use in your project, add the `simplegui.h`
file, link to `simplegui.lib`, and copy `simplegui.dll` to your executable
directory.

## Creating a Window

Use `simplegui::Window::Create(int width, int height, const char *title)` to create a window. The function returns a `Window *`. The window is not shown
until `Window::Show(bool show)` is called. The window must be destroyed
using the `delete` operator. An example is below.

```cpp
#include <simplegui.h>

using namespace simplegui;

const int WIDTH = 600;
const int HEIGHT = 800;
const char *TITLE = "My Window";

int main(int argc, char *argv[])
{
    // create window with width, height, and title
    Window *window = Window::Create(WIDTH, HEIGHT, TITLE);

    window->Show(true); // show the window

    window->Wait(); // wait for the window to close
    delete window; // release the window

    return 0;
}
```

## Handling Events

The following event handlers exist:

* `KeyListener`
* `MouseListener`
* `WindowListener`

Beware! The event handlers are called on a separate thread!

Example usage of a `KeyListener` to close the window when escape key
is pressed:

```cpp
// ...

// define a class that inherits from KeyListener
class MyListener : public KeyListener
{
public:
    // called when a key is pressed
    virtual void KeyDown(Window *win, int vk) override
    {
        if (vk == KEY_ESCAPE)
            win->Dispose(); // dispose the window
    }
};

int main(int argc, char *argv[])
{
    // ...

    MyListener listener;
    window->SetKeyListener(&listener); // set the key listener (before showing)

    // ...
}
```

See the class definitions for more events that can be handled.

## Drawing

Drawing is also an event-based process. To do custom drawing, an implementation
of a `Painter` must be provided.

```cpp
// ...

// define a class that inherits from Painter
class MyPainter : public Painter
{
public:
    // called when the window is painted
    virtual void Paint(Window *win, Graphics *g) override
    {
        g->Clear(); // clear the window

        g->SetColor(255, 0, 0); // set the color to red
        g->FillRect(0, 0, 100, 100); // fill a rectangle
    }
};

int main(int argc, char *argv[])
{
    // ...

    MyPainter painter;
    window->SetPainter(&painter); // set the painter (before showing)

    // ...
}
```

See the `Graphics` class for more drawing functions.

Like events, painting occurs on a separate thread. The `Graphics` class is
only valid during the `Paint` function call. Do not store a reference to the
`Graphics` object. Painting may occur at any time. To suggest a repaint of
the window from another thread, call `Window::Invalidate()`, which will trigger
a repaint of the window in the future.