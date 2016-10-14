/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <gtk/gtk.h>

class Context;

namespace signals {
    void set_context(Context&);
    int run(int, char**);
    
    void activate();
    void close();
    void pause();
    void start();
    void stop();

    bool configure_animation(GtkWidget*, GdkEventConfigure*);
}

#endif /* SIGNALS_HPP */