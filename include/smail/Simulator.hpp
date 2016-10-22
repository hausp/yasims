
#ifndef SMAIL_SIMULATOR_HPP
#define SMAIL_SIMULATOR_HPP

#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mutex>

#include "animation/Animation.hpp"
#include "Config.hpp"
#include "Default.hpp"
#include "Event.hpp"
#include "MessageClassifier.hpp"
#include "MessageConsumer.hpp"
#include "MessageProducer.hpp"
#include "ProcessingCenter.hpp"

namespace smail {
    class Simulator {
        using EventQueue = std::priority_queue<Event,
            std::deque<Event>, std::greater<Event>>;
     public:
        enum class Instance { UNIQUE };

        Simulator();
        ~Simulator();

        void start(bool = false);
        void step();
        void pause();
        void stop();

        void update_speed(double);
        void update_config(Config);
     private:
        Config config;
        EventQueue events;
        Animation animation;
        std::array<MessageProducer, 2> spawners;
        MessageClassifier classifier;
        ProcessingCenter reception;
        std::array<ProcessingCenter, 2> centers;
        MessageConsumer consumer;
        double clock = 0;
        double speed = Default::SPEED_NUMERATOR/Default::SPEED_DENOMINATOR;

        // Statistics
        unsigned msgs_in_system = 0;
        unsigned max_msgs_in_system = 0;
        unsigned min_msgs_in_system = 10;
        double system_occupation = 0;
        double reception_occupation = 0;
        double reception_queue_occu = 0;
        std::array<double, 2> occupation_on_centers = {0, 0};
        std::array<double, 2> occupation_on_queues = {0, 0};
        std::unordered_map<Message,size_t> input_info;


        // Thread related attributes
        std::mutex mutex;
        std::condition_variable cv;
        std::thread thread;
        unsigned gtk_handler;
        bool execute = false;
        bool stopped = false;
        bool survive = true;       

        // Running methods, first one with particular thread,
        // latter one with gtk's thread
        void run();
        bool gtk_run();

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
        void launch_gtk_function();
        void show_statistics();
        void update_messages();
        void update_input_info(const Message&);
        std::stringstream avg_occupation(size_t);
        std::stringstream reveal_input_info();
        std::stringstream reveal_messages_info();
        size_t get_input_info(Address, Address);
        void generate_output(const std::string&);
    };
}

#endif /* SMAIL_SIMULATOR_HPP */
