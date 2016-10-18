
#include "core/Simulator.hpp"
#include "random/Function.hpp"
#include <iostream>

Simulator::Simulator():
 arrival_times{{
    {Address::LOCAL, dist::expo(0.6)},
    {Address::REMOTE, dist::expo(0.5)}
 }},
 thread{&Simulator::run, this} { }

Simulator::~Simulator() {
    survive = false;
    cv.notify_all();

    if (thread.joinable()) {
        thread.join();
    }
}

void Simulator::start(bool a) {
    if (execute) return;
    animate = a;

    if (animate) animation.start();

    std::lock_guard<std::mutex> guard{mutex};
    execute = true;
    setup();
    cv.notify_all();
}

void Simulator::pause() {
    if (!execute) return;
    if (animate) animation.pause();

    execute = false;
}

void Simulator::stop() {
    if (!execute) return;
    if (animate) animation.stop();

    reset();
    setup();
    execute = false;
}

void Simulator::run() {
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

void Simulator::setup() {
    if (events.empty()) {
        create_arrival_event(Address::LOCAL);
        create_arrival_event(Address::REMOTE);
    }
}

void Simulator::reset() {
    auto cleaner = EventQueue{};
    events.swap(cleaner);
    // TODO: animation
}

void Simulator::create_arrival_event(Address addr) {
    auto event = Event{
        current_time + arrival_times.generate(addr),
        message_factory.create(addr),
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

void Simulator::create_reception_event(const Message& message) {
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

void Simulator::create_processing_event(const Message& message) {
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

void Simulator::create_postpone_event(const Message& message) {
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

void Simulator::create_exit_event(const Message& message) {
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