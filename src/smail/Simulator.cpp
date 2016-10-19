
#include "smail/Default.hpp"
#include "smail/Simulator.hpp"
#include "random/Function.hpp"
#include <iostream>

smail::Simulator::Simulator():
 spawners{
    MessageProducer{
        Address::LOCAL,
        Default::L_ARRIVAL_TIMES,
        Default::L_DESTINATION
    },
    MessageProducer{
        Address::REMOTE,
        Default::R_ARRIVAL_TIMES,
        Default::R_DESTINATION
    }
 },
 classifier{
    Default::L_STATUS_WEIGHTS,
    Default::R_STATUS_WEIGHTS,
 },
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
            e.pre_action(e.time);
            // animation.wait_for(e.message().from)
            e.pos_action(e.time);

            events.pop();
            execute = !events.empty();
        }
    }
}

void smail::Simulator::setup() {
    if (events.empty()) {
        arrival_event(static_cast<size_t>(Address::LOCAL));
        arrival_event(static_cast<size_t>(Address::REMOTE));
    }
}

void smail::Simulator::reset() {
    auto cleaner = EventQueue{};
    events.swap(cleaner);
    // TODO: animation
}

void smail::Simulator::arrival_event(size_t index) {
    // Produce message and its arrival event
    auto event = spawners[index].produce(clock, seed);
    // Set arrival event pre_action (before animation)
    event.pre_action = [this, index](double time) {
        // Update simulation clock
        clock = time;
        // Creates next arrival event
        arrival_event(index);
    };
    // Set arrival event pos_action (after animation)
    event.pos_action = [this, index](double time) {
        // Dispatch a message from the spawner
        auto msg = spawners[index].dispatch();
        // Classify this message (choose its status)
        classifier.classify(msg, seed);
        // Send message to reception and create a new reception event
        reception_event(std::move(msg));
    };

    events.push(event);
}

void smail::Simulator::reception_event(Message msg) {
    // TODO
}

void smail::Simulator::processing_event() {
    // TODO
}

void smail::Simulator::postpone_event() {
    // TODO
}

void smail::Simulator::exit_event() {
    // TODO
}