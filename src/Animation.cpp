
#include <iostream>
#include "Animation.hpp"
#include "utils.hpp"
#include <gtk/gtk.h>

void Animation::stop() {
    animate = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void Animation::pause() {
    animate = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void Animation::start() {
    if (!animate) {
        thread = std::thread(&Animation::run, this);
    }
    animate = true;
}

void Animation::run() {
    static int x = 0, y = 0;
    while (animate) {
        if (cairo.done()) {
            cairo.move_to(x, y);
            y = (y + 30) % 630;
            if (y == 0) {
                x = (x + 20) % 820;
                if (x == 0) {
                    break;
                }
            }
            cairo.line_to(x, y);
            cairo.stroke();
            cairo.draw_request();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }
}

void Animation::join() {
    thread.join();
}