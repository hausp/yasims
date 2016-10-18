
#include <iostream>
#include "animation/Animation.hpp"
#include "wrapper/Signal.hpp"

Animation::Animation() {
    aw::Signal<Animation>::set_receiver(*this);
}

void Animation::start() {
    if (!animating) {
        animating = true;
        g_idle_add_full(
            G_PRIORITY_HIGH_IDLE + 25,
            (GSourceFunc)
                (aw::Signal<Animation>::function<bool()>
                    ::callback<&Animation::run>),
            nullptr,
            nullptr
        );
    }
}

void Animation::pause() {
    animating = false;
}

void Animation::stop() {
    animating = false;
}

bool Animation::run() {
    if (cairo.drawed()) {
        animating = update();
        cairo.queue_draw();
    }
    return animating;
}


bool Animation::update() {
    static int x = 0, y = 0;
    cairo.move_to(x, y);
    y = (y + 30) % 630;
    if (y == 0) {
        x = (x + 20) % 820;
        if (x == 0) {
            return false;
        }
    }
    cairo.line_to(x, y);
    cairo.stroke();
    return true;
}

bool Animation::ready(const Message* index) const {
    return true;
}