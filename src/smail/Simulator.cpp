
#include <cassert>
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
        Default::R_PROCESSING_TIMES,
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

void smail::Simulator::start(bool anima) {
    if (execute) return;
    if (stopped) reset();

    setup();

    execute = true;
    animate = anima;  

    if (animate) {
        animation.start();
        // g_timeout_add_full(

        // );

    } else {
        std::lock_guard<std::mutex> guard{mutex};        
        cv.notify_all();
    }
}

void smail::Simulator::step() {
    if (execute) return;
    if (stopped) reset();
    animate = true;
    
    setup();

    simulate();
}

void smail::Simulator::pause() {
    if (!execute) return;

    if (animate) animation.pause();
    
    execute = false;
}

void smail::Simulator::stop() {
    // If not executing, nothing to be done
    if (!execute) return;
    // Stop animation, if any
    if (animate) animation.stop();
    // Sinalize to stop executing
    execute = false;
    stopped = true;
    consumer.reveal_info();
    std::cout << "There are still: " << msgs_in_system 
        << " messages in the system\n";
}

void smail::Simulator::run() {
    while (survive) {
        std::unique_lock<std::mutex> permission{mutex};
        // Synchronizing operations, to keep thread suspended
        // when paused and stoped
        cv.wait(permission, [this] { return !survive || execute; });
        permission.unlock();
        // execution loop
        while (execute && survive) {
            simulate();
        }
    }
}

// For animations, better be in gtk's thread
void smail::Simulator::gtk_run() {
    // TODO
}

void smail::Simulator::simulate() { 
    std::cout << "------------------------------------" << std::endl;
    // Get next event
    auto e = events.top();
    // Update simulation clock and statistics
    update(e.time); // NOTICE: if e.action() fails this will pass equal times
    // Execute event action and check if successful
    if (e.action()) {
        // Successful events are removed
        events.pop();
    }
    // Update execute control variable
    stopped = events.empty();
    execute = execute && !stopped;
}

void smail::Simulator::update(double new_time) {
    auto occupation = reception.occupation();
    for (auto& center : centers) {
        occupation += center.occupation();
    }
    if (!system_occupation.count(occupation)) {
        system_occupation[occupation] = 0;    
    }
    system_occupation[occupation] += (new_time - clock);
    clock = new_time;
}

void smail::Simulator::setup() {
    // If there is no event
    if (events.empty()) {
        // Give the initial stimulus, one local arrive and one remote
        arrival_event(static_cast<size_t>(Address::LOCAL));
        arrival_event(static_cast<size_t>(Address::REMOTE));
    }
}

void smail::Simulator::reset() {
    // A brand new EventQueue to replace older one
    auto cleaner = EventQueue{};
    events.swap(cleaner);
    // TODO: animation
    reception.reset();
    classifier.reset();
    consumer.reset();
    msgs_in_system = 0;

    for (auto& spawner : spawners) {
        spawner.reset();
    }

    for (auto& center : centers) {
        center.reset();
    }
}

void smail::Simulator::arrival_event(size_t index) {
    std::cout << "Arrival event" << std::endl;
    // Produce message and its arrival event
    auto event = spawners[index].produce(clock);
    // Set arrival event action
    event.action = [this, index] {
        // Creates next arrival event
        arrival_event(index);
        // Dispatch a message from the spawner
        auto msg = spawners[index].dispatch();
        // Classify this message (choose its status)
        classifier.classify(msg);
        // Send message to reception
        reception_event(std::move(msg));
        // TODO
        return true;
    };
    events.push(std::move(event));
}

void smail::Simulator::reception_event(Message msg) {
    std::cout << "Reception event" << std::endl;
    // Increment #msgs in system
    msgs_in_system++;
    // Send message to reception and get ready event
    auto event = reception.receive(std::move(msg));
    // Set ready event action
    event.action = [this] {
        // Get dispatched message
        Message msg;
        bool dispatched;
        std::tie(msg, dispatched) = reception.dispatch();
        // temporary assert
        assert(dispatched);
        // Send message to processing centers
        processing_event(std::move(msg));
        // TODO
        return true;
    };
    events.push(std::move(event));
}

void smail::Simulator::processing_event(Message msg) {
    std::cout << "Processing event" << std::endl;
    // Get index for the corresponding center
    auto index = static_cast<size_t>(msg.from);
    std::cout << "index: " << index << std::endl;
    // Send message to center and get ready event
    auto event = centers[index].receive(std::move(msg));
    // Set ready event action
    event.action = [this, index] {
        // Get dispatched message
        Message msg;
        bool dispatched;
        std::tie(msg, dispatched) = centers[index].dispatch();
        // temporary assert
        assert(dispatched);
        // Check if has been postponed
        if (msg.status != Status::POSTPONED) {
            // If not, send message to exit
            exit_event(std::move(msg));
        } else {
            // Otherwise, send to postpone treatment
            postpone_event(std::move(msg));
        }
        // TODO
        return true;
    };
    events.push(std::move(event));
}

void smail::Simulator::postpone_event(Message msg) {
    std::cout << "Postpone event" << std::endl;
    if (msg.in_system_time >= message_timeout) {
        // Timeout = FAILURE
        msg.status = Status::FAILURE;
        // Send message to exit
        exit_event(std::move(msg));
    } else {
        // Reclassify the message
        classifier.classify(msg);
        // Resend to processing centers
        processing_event(std::move(msg));
    }
}

void smail::Simulator::exit_event(Message msg) {
    std::cout << "Exit event" << std::endl;
    // Decrement #msgs in system
    msgs_in_system--;
    // Process dispatched msg data
    consumer.consume(msg);
}