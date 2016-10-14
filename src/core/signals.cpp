/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "core/signals.hpp"
#include "core/Context.hpp"

#include <iostream>

// Anonymous namespace, only this file can see it
namespace {
    Context* receiver;
}

void signals::set_context(Context& c) {
    static bool first = true;
    if (first) {
        receiver = &c;
        first = false;
    }
}

int signals::run(int argc, char** argv) {
    return receiver->run(argc, argv);
}

void signals::activate() {
    receiver->activate();
}

void signals::close() {
    std::cout << "[signals] close" << std::endl;
    receiver->close();
}

void signals::pause() {
    receiver->pause();
}

void signals::start() {
    receiver->start();
}

void signals::stop() {
    receiver->stop();
}

bool signals::configure_animation(GtkWidget* widget, GdkEventConfigure*) {
    return receiver->configure_animation(widget);
}
