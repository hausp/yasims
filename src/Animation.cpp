
#include <iostream>
#include "Animation.hpp"
#include "core/signals.hpp"
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
    while (animate) {
        std::cout << "animating..." << std::endl;
    }
}

void Animation::draw() {
    
}

void Animation::join() {
    thread.join();
}

bool Animation::configure(GtkWidget* widget) {
    std::cout << "configure" << std::endl;
    cairo.update(widget);
    return true;
}