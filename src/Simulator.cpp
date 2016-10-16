
#include "Simulator.hpp"
#include <iostream>

void Simulator::start() {
    std::cout << "[sim] start" << std::endl;
    animation.start();
    std::cout << "[sim] end of start" << std::endl;
}

void Simulator::pause() {
    std::cout << "[sim] pause" << std::endl;
    animation.pause();
}

void Simulator::stop() {
    std::cout << "[sim] stop" << std::endl;
    animation.stop();
}

void Simulator::destroy() {
    std::cout << "[sim] destroy" << std::endl;
    animation.stop();
}
