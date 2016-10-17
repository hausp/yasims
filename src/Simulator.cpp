
#include "Simulator.hpp"
#include "random/Function.hpp"
#include <iostream>

Simulator::Simulator():
 // TODO: remove 0.5 hardcoded
 generator(dist::expo(1, 0.5)) { }

void Simulator::start() {
    animation.start();
}

void Simulator::pause() {
    animation.pause();
}

void Simulator::stop() {
    animation.stop();
}

void Simulator::destroy() {
    animation.stop();
}

void Simulator::run() {

}

void Simulator::generate_input(double last_time) {
    auto event = generator.generate(
        last_time,
        [this](const Message& message, double time) {
            generate_input(time);
            // MAYBE TODO: use condition_variable to wait
            while(animate && !animation.ready(&message));
        }
    );

    events.push(event);
}
