
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <gtk/gtk.h>
#include "wrapper/Cairo.hpp"
#include "wrapper/Signal.hpp"

class Animation {
 public:
    enum class Instance { UNIQUE };

    Animation();

    void start();
    void pause();
    void stop();
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
