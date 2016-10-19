
#ifndef SMAIL_SIMULATOR_HPP
#define SMAIL_SIMULATOR_HPP

#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>

#include "animation/Animation.hpp"

#include "Event.hpp"
#include "MessageConsumer.hpp"
#include "MessageProducer.hpp"

namespace smail {
    class Simulator {
        using EventQueue = std::priority_queue<Event, std::deque<Event>>;
     public:
        Simulator();
        ~Simulator();

        void start(bool = false);
        void pause();
        void stop();

        void run();
        void setup();
        void reset();
     private:
        EventQueue events;
        Animation animation;
        MessageProducer local;
        MessageProducer remote;
        // MessageFactory message_factory;
        double current_time = 0;
        int seed = 0;

        // Thread related attributes
        std::mutex mutex;
        std::condition_variable cv;
        std::thread thread;
        bool animate = false;
        bool execute = false;
        bool survive = true;

        void create_arrival_event(Address);
        void create_reception_event(const Message&);
        void create_processing_event(const Message&);
        void create_postpone_event(const Message&);
        void create_exit_event(const Message&);
    };
}

#endif /* SMAIL_SIMULATOR_HPP */
