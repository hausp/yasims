
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
#include "Default.hpp"

namespace smail {
    class Simulator {
        using EventQueue = std::priority_queue<Event,
            std::deque<Event>, std::greater<Event>>;
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
        ProcessingCenter reception;
        std::array<ProcessingCenter, 2> centers;
        // MessageConsumer dispatcher;
        double message_timeout = Default::TIME_OUT;
        double clock = 0;

        // Statistics
        std::unordered_map<unsigned, double> system_occupation;

        // Thread related attributes
        std::mutex mutex;
        std::condition_variable cv;
        std::thread thread;
        bool animate = false;
        bool execute = false;
        bool stopped = false;
        bool survive = true;

        void update(double);
        void arrival_event(size_t);
        void reception_event(Message);
        void processing_event(Message);
        void postpone_event(Message);
        void exit_event(Message);
    };
}

#endif /* SMAIL_SIMULATOR_HPP */
