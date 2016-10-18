
#include "core/Simulator.hpp"
#include "random/Function.hpp"
#include <iostream>

Simulator::Simulator():
 // TODO: remove 1 and 0.5 hardcoded
 generator(dist::expo(1, 0.5)),
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

void Simulator::generate_input(double last_time) {
    auto event = generator.generate(
        last_time,
        [this](const Message& message, double time) {
            generate_input(time);
            // MAYBE TODO: use condition_variable to wait
            // while(animate && !animation.ready(&message));
        }
    );

    events.push(event);
}
