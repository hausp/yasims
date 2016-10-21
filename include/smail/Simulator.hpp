
#ifndef SMAIL_SIMULATOR_HPP
#define SMAIL_SIMULATOR_HPP

#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>

#include "animation/Animation.hpp"
#include "Config.hpp"
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
        void step();
        void pause();
        void stop();
     private:
        Config config;
        EventQueue events;
        Animation animation;
        std::array<MessageProducer, 2> spawners;
        MessageClassifier classifier;
        ProcessingCenter reception;
        std::array<ProcessingCenter, 2> centers;
        MessageConsumer consumer;
        // double message_timeout = Default::TIME_OUT;
        double clock = 0;
        unsigned msgs_in_system = 0;
        unsigned max_msgs_in_system = 0;
        unsigned min_msgs_in_system = 10;

        // Statistics
        double system_occupation = 0;

        // Thread related attributes
        std::mutex mutex;
        std::condition_variable cv;
        std::thread thread;
        bool animate = false;
        bool execute = false;
        bool stopped = false;
        bool survive = true;
        std::array<double, 2> occupation_on_centers = {0, 0};
        std::array<double, 2> occupation_on_queues = {0, 0};
        std::unordered_map<Message,size_t> input_info;

        // Running methods, first one with particular thread,
        // latter one with gtk's thread
        void run();
        void gtk_run();

        // Auxiliar methods
        void setup();
        void reset();
        void simulate();
        void update(double);
        void arrival_event(size_t);
        void reception_event(Message);
        void processing_event(Message);
        void postpone_event(Message);
        void exit_event(Message);
        void update_messages();
        void reveal_messages_info();
        void avg_occupation(size_t);
        void update_input_info(const Message&);
        void reveal_input_info();
    };
}

#endif /* SMAIL_SIMULATOR_HPP */
