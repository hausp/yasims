

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>

#include "animation/Animation.hpp"
#include "Event.hpp"
#include "Generator.hpp"

class Simulator {
    using EventQueue = std::priority_queue<Event, std::deque<Event>>;
 public:
    Simulator();
    ~Simulator();

    void start(bool = false);
    void pause();
    void stop();

    void run();
    void generate_input(double);
 private:
    EventQueue events;
    Generator generator;
    Animation animation;
    std::mutex mutex;
    std::condition_variable cv;
    std::thread thread;
    bool animate = false;
    bool execute = false;
    bool survive = true;
};

#endif /* SIMULATOR_HPP */
