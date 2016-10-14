/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "core/Context.hpp"
#include <iostream>

void Context::activate() {
    gui.activate();
    // Called when g_application_run do its stuff
    // Can be used to initialize other shits
}

int Context::run(int argc, char** argv) {
    return g_application_run(gui(), argc, argv);
}

void Context::start() {
    sim.start();
}

void Context::pause() {
    sim.pause();
}

void Context::stop() {
    sim.stop();
}

void Context::close() {
    sim.destroy();
    gui.destroy();
}

bool Context::configure_animation(GtkWidget* widget) {
    return sim.configure_animation(widget);
}