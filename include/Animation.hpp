
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <gtk/gtk.h>
#include <thread>
#include "wrapper/Cairo.hpp"

class Animation {
 public:

    void pause();
    void start();
    void stop();
    void join();

    bool configure(GtkWidget*);
 private:
    Cairo cairo;
    std::thread thread;
    bool animate = false;
    
    void run();
};

#endif /* ANIMATION_HPP */
