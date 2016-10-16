

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "Animation.hpp"

class Simulator {
 public:

    void destroy();
    void start();
    void pause();
    void stop();
 private:
    bool execute = false;
    std::thread thread;
    Animation animation;
};

#endif /* SIMULATOR_HPP */
