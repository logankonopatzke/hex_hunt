#include "cell_movement.hpp"

#ifdef __linux__
#include <X11/Xlib.h>
#include "X11/keysym.h"

#define KEY_UP XK_Up
#define KEY_DOWN XK_Down
#define KEY_LEFT XK_Left
#define KEY_RIGHT XK_Right

#elif _WIN32
#include <Windows.h>

#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT

#endif

namespace hh
{

#ifdef __linux__
    /*
    Function inspired by https://stackoverflow.com/a/52801588
    */
    inline static bool check_key_press(KeySym key_stroke)
    {
        Display *dpy = XOpenDisplay(":0"); // Open handle to primary display (display/window 0)

        char keys_return[32];
        XQueryKeymap(dpy, keys_return);
        KeyCode kc2 = XKeysymToKeycode(dpy, key_stroke);
        bool is_pressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));

        XCloseDisplay(dpy); // Close handle

        return is_pressed;
    }
#elif _WIN32
    inline static bool check_key_press(int key_stroke)
    {
        return GetAsyncKeyState(key_stroke) & 0x8000; // Await key held (bitmask)
    }
#endif

    bool cell_movement::move_up()
    {
        return check_key_press(KEY_UP);
    }

    bool cell_movement::move_down()
    {
        return check_key_press(KEY_DOWN);
    }

    bool cell_movement::move_left()
    {
        return check_key_press(KEY_LEFT);
    }

    bool cell_movement::move_right()
    {
        return check_key_press(KEY_RIGHT);
    }

    bool cell_movement::move_any()
    {
        return move_up() || move_down() || move_left() || move_right();
    }
} // namespace hh