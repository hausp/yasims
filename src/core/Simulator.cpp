
#include "core/Simulator.hpp"
#include "random/Function.hpp"
#include <iostream>

Simulator::Simulator():
 local(message_factory, dist::expo(0.6)),
 remote(message_factory, dist::expo(0.5)),
 thread(&Simulator::run, this) { }

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
        }
    }
}

void Simulator::generate_input() {
    auto event_action = [this](const Message& message, double time) {
        current_time = time;
        generate_input();
        // MAYBE TODO: use condition_variable to wait
        // while(animate && !animation.ready(&message));
    };

    auto event = local.create(current_time, event_action);
    events.push(event);

    event = remote.create(current_time, event_action);
    events.push(event);
}
