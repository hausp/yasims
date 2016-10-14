/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <thread>
#include "Animation.hpp"
#include "GTKInterface.hpp"
#include "Simulator.hpp"

class Context {
 public:
    int run(int, char**);

    void activate();
    void close();
    void pause();
    void start();
    void stop();

    bool configure_animation(GtkWidget*);

 private:
    Simulator sim;
    GTKInterface gui;
};

#endif /* CONTEXT_HPP */
