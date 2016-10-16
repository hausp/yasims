
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
        std::cout << "[Animation] start" << std::endl;
        animate = true;
        thread = std::thread(&Animation::run, this);
    }
     std::cout << "[Animation] end of start" << std::endl;
}

void Animation::run() {
    std::cout << "[Animation] run" << std::endl;
    // Sanity sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    static int x = 0, y = 0;
    while (animate) {
        std::cout << "[Animation] animate loop" << std::endl;
        if (cairo.done()) {
            std::cout << "[Animation] cairo done" << std::endl;
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