
#include "smail/Default.hpp"
#include "smail/Simulator.hpp"
#include "random/Function.hpp"
#include <iostream>

smail::Simulator::Simulator():
 arrival_times{smail::Default::arrival_times},
 reception_times{smail::Default::reception_times},
 processing_times{smail::Default::processing_times},
 thread{&smail::Simulator::run, this} { }

smail::Simulator::~Simulator() {
    survive = false;
    cv.notify_all();

    if (thread.joinable()) {
        thread.join();
    }
}

void smail::Simulator::start(bool a) {
    if (execute) return;
    animate = a;

    if (animate) animation.start();

    std::lock_guard<std::mutex> guard{mutex};
    execute = true;
    setup();
    cv.notify_all();
}

void smail::Simulator::pause() {
    if (!execute) return;
    if (animate) animation.pause();

    execute = false;
}

void smail::Simulator::stop() {
    if (!execute) return;
    if (animate) animation.stop();

    reset();
    setup();
    execute = false;
}

void smail::Simulator::run() {
    while (survive) {
        std::unique_lock<std::mutex> permission{mutex};
        cv.wait(permission, [this] {
            return !survive || execute;
        });
        permission.unlock();

        while (execute && survive) {
            std::cout << "[Simulator] executing" << std::endl;
            auto e = events.top();
            e.pre_action(e.message, e.time);
            // animation.wait_for(e.message().from)
            e.pos_action(e.message, e.time);

            events.pop();
            execute = !events.empty();
        }
    }
}

void smail::Simulator::setup() {
    if (events.empty()) {
        create_arrival_event(Address::LOCAL);
        create_arrival_event(Address::REMOTE);
    }
}

void smail::Simulator::reset() {
    auto cleaner = EventQueue{};
    events.swap(cleaner);
    // TODO: animation
}

void smail::Simulator::create_arrival_event(Address addr) {
    auto event = Event{
        current_time + arrival_times.generate(addr),
        // message_factory.create(addr),
        Message(),
        [this](const Message& message, double time) {
            current_time = time;
            create_arrival_event(message.from);
        },
        [this](const Message& message, double time) {
            create_reception_event(message);
        }
    };
    events.push(event);
}

void smail::Simulator::create_reception_event(const Message& message) {
    auto placeholder = 0;
    auto event = Event{
        current_time + placeholder,
        message,
        [this](const Message& message, double time) {
            current_time = time;
            
        },
        [this](const Message& message, double time) {
            create_processing_event(message);
        }
    };
    events.push(event);
}

void smail::Simulator::create_processing_event(const Message& message) {
    auto placeholder = 0;
    auto event = Event{
        // TODO
        current_time + placeholder,
        message,
        [this](const Message& message, double time) {
            current_time = time;
            switch(message.status) {
                case Status::POSTPONED:
                    create_postpone_event(message);
                    break;
                default: // Covers both success and failure.
                    create_exit_event(message);
                    break;
            }
        },
        [this](const Message& message, double time) {
            // TODO
        }
    };
    events.push(event);
}

void smail::Simulator::create_postpone_event(const Message& message) {
    auto placeholder = 0;
    auto event = Event{
        // TODO
        current_time + placeholder,
        message,
        [this](const Message& message, double time) {
            current_time = time;
            // TODO
        },
        [this](const Message& message, double time) {
            // TODO
        }
    };
    events.push(event);
}

void smail::Simulator::create_exit_event(const Message& message) {
    auto placeholder = 0;
    auto event = Event{
        current_time + placeholder,
        message,
        [this](const Message& message, double time) {
            current_time = time;
            // TODO
        },
        [this](const Message& message, double time) {
            // TODO
        }
    };
    events.push(event); 
}