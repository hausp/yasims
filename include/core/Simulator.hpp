

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>

#include "animation/Animation.hpp"
#include "Event.hpp"
#include "EventFactory.hpp"
#include "MessageFactory.hpp"

class Simulator {
    using EventQueue = std::priority_queue<Event, std::deque<Event>>;
 public:
    Simulator();
    ~Simulator();

    void start(bool = false);
    void pause();
    void stop();

    void run();
    void generate_input();
 private:
    EventQueue events;
    Animation animation;
    MessageFactory message_factory;
    EventFactory local;
    EventFactory remote;
    double current_time = 0;

    // Thread related attributes
    std::mutex mutex;
    std::condition_variable cv;
    std::thread thread;
    bool animate = false;
    bool execute = false;
    bool survive = true;
};

#endif /* SIMULATOR_HPP */
