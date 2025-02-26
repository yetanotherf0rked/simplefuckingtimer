#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <iostream>
#include "timer.h"
#include <thread>
#include <unistd.h>

// Declare external Timer instance.
extern Timer timer;

void hotkeyListener() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Cannot open X display\n";
        return;
    }
    Window root = DefaultRootWindow(display);

    KeyCode insertKey = XKeysymToKeycode(display, XK_Insert);
    KeyCode pauseKey  = XKeysymToKeycode(display, XK_Pause);
    KeyCode homeKey   = XKeysymToKeycode(display, XK_Home); // New HOME key

    // Grab keys on the root window.
    XGrabKey(display, insertKey, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, pauseKey, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, homeKey, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    XSelectInput(display, root, KeyPressMask);

    XEvent event;
    while (true) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            XKeyEvent xkey = event.xkey;
            if (xkey.keycode == insertKey) {
                TimerState state = timer.getState();
                if (state == STOPPED) {
                    timer.start();
                } else if (state == RUNNING) {
                    timer.split();
                } else if (state == FINISHED) {
                    timer.reset();
                }
            } else if (xkey.keycode == pauseKey) {
                TimerState state = timer.getState();
                if (state == RUNNING) {
                    timer.pause();
                } else if (state == PAUSED) {
                    timer.resume();
                }
            } else if (xkey.keycode == homeKey) {
                // New: If the run is finished, revert it (resume).
                TimerState state = timer.getState();
                if (state == FINISHED) {
                    timer.revertFinish();
                }
            }
        }
        usleep(10000);
    }
    XCloseDisplay(display);
}

