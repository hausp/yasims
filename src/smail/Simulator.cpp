
#include <cassert>
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
 reception{
    Default::RECEPTION_TIMES
 },
 centers {
    ProcessingCenter{
        Default::L_PROCESSING_TIMES,
        Default::LOCAL_CENTER_CAPACITY
    },
    ProcessingCenter{
        Default::L_PROCESSING_TIMES,
        Default::REMOTE_CENTER_CAPACITY
    }
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
            e.action(e.time);
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
    auto event = spawners[index].produce(clock);
    // Set arrival event action
    event.action = [this, index](double time) {
        // Update simulation clock
        clock = time;
        // Creates next arrival event
        arrival_event(index);
        // Dispatch a message from the spawner
        auto msg = spawners[index].dispatch();
        // Classify this message (choose its status)
        classifier.classify(msg);
        // Send message to reception
        reception_event(std::move(msg));
    };
    events.push(std::move(event));
}

void smail::Simulator::reception_event(Message msg) {
    auto event = reception.receive(std::move(msg));
    event.action = [this](double time) {
        // Update simulation clock
        clock = time;
        // Get dispatched message
        Message msg;
        bool dispatched;
        std::tie(msg, dispatched) = reception.dispatch();
        // temporary assert
        assert(dispatched);
        // Send message to processing centers
        processing_event(std::move(msg));
    };
    events.push(std::move(event));
}

void smail::Simulator::processing_event(Message msg) {
    // TODO
}

void smail::Simulator::postpone_event(Message msg) {
    // TODO
}

void smail::Simulator::exit_event(Message msg) {
    // TODO
}