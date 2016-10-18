

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <queue>
#include <deque>

#include "animation/Animation.hpp"
#include "Event.hpp"
#include "Generator.hpp"

class Simulator {
    using EventQueue = std::priority_queue<Event, std::deque<Event>>;
 public:
    Simulator();
    ~Simulator();

    void start();
    void pause();
    void stop();

    void run();
    void generate_input(double);
 private:
    EventQueue events;
    Generator generator;
    bool execute = false;
    std::thread thread;
    Animation animation;
    bool animate;
};

#endif /* SIMULATOR_HPP */
