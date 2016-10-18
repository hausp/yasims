
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <atomic>
#include <condition_variable>
#include <gtk/gtk.h>
#include <thread>
#include "wrapper/Cairo.hpp"
#include "core/Message.hpp"
#include "wrapper/Signal.hpp"

class Animation {
 public:
    enum class Instance { UNIQUE };

    Animation();

    void start();
    void pause();
    void stop();

    bool ready(const Message *) const;
 private:
    Cairo cairo;
    bool animating = false;
    
    bool run();
    void reset();
    bool update();
    
    // Ma man
    friend class aw::Signal<Animation>;
};

#endif /* ANIMATION_HPP */
