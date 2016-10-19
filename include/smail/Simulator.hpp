
#ifndef SMAIL_SIMULATOR_HPP
#define SMAIL_SIMULATOR_HPP

#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>

#include "animation/Animation.hpp"
#include "Event.hpp"
#include "MessageClassifier.hpp"
#include "MessageConsumer.hpp"
#include "MessageProducer.hpp"
#include "ProcessingCenter.hpp"

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
        std::array<MessageProducer, 2> spawners;
        MessageClassifier classifier;
        double current_time = 0;
        int seed = 0;

        // Thread related attributes
        std::mutex mutex;
        std::condition_variable cv;
        std::thread thread;
        bool animate = false;
        bool execute = false;
        bool survive = true;

        void arrival_event(size_t);
        void reception_event(Message);
        void processing_event();
        void postpone_event();
        void exit_event();
    };
}

#endif /* SMAIL_SIMULATOR_HPP */
