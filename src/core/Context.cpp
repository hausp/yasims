/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "core/Context.hpp"
#include "wrapper/Signal.hpp"
#include <iostream>

Context::Context() {
    aw::Signal<Context>::set_receiver(*this);
}

int Context::run(int argc, char** argv) {
    return g_application_run(gui(), argc, argv);
}

void Context::start() {
    std::cout << "[Context] start" << std::endl;
    sim.start(true);
    std::cout << "[Context] end of start" << std::endl;
}

void Context::pause() {
    sim.pause();
}

void Context::stop() {
    sim.stop();
}

void Context::close() {
    std::cout << "hello darkness" << std::endl;
    gui.destroy();
}
