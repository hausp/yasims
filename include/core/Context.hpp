/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <thread>
#include "interface/GTKInterface.hpp"
#include "smail/Simulator.hpp"

class Context {
 public:
    enum class Instance { UNIQUE };

    Context();

    int run(int, char**);

    void activate();
    void close();
    void pause();
    void start();
    void stop();
 private:
    smail::Simulator sim;
    GTKInterface gui;
};

#endif /* CONTEXT_HPP */
