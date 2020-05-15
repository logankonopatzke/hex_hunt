#include "cell_movement.hpp"

#include <X11/Xlib.h>
#include "X11/keysym.h"

namespace hh
{
    /*
    Function inspired by https://stackoverflow.com/a/52801588
    */
    inline static bool check_key_press(KeySym ks)
    {
        Display *dpy = XOpenDisplay(":0");
        char keys_return[32];
        XQueryKeymap(dpy, keys_return);
        KeyCode kc2 = XKeysymToKeycode(dpy, ks);
        bool is_pressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));

        XCloseDisplay(dpy);

        return is_pressed;
    }

    bool cell_movement::move_up()
    {
        return check_key_press(XK_Up);
    }

    bool cell_movement::move_down()
    {
        return check_key_press(XK_Down);
    }

    bool cell_movement::move_left()
    {
        return check_key_press(XK_Left);
    }

    bool cell_movement::move_right()
    {
        return check_key_press(XK_Right);
    }

    bool cell_movement::move_any()
    {
        return move_up() || move_down() || move_left() || move_right();
    }
}