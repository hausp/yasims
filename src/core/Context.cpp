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

void Context::configure() {
    gui.show_configuration_dialog();
}

void Context::summary() {
    // TODO
}

void Context::start() {
    sim.start(true);
    gui.update_active_buttons(true);
}

void Context::fast_forward() {
    sim.start(false);
    gui.update_active_buttons(true);
}

void Context::step() {
    sim.step();
}

void Context::pause() {
    sim.pause();
    gui.update_active_buttons(false);
}

void Context::stop() {
    sim.stop();
    gui.update_active_buttons(false);
}

void Context::close() {
    gui.destroy();
}

void Context::change_speed(GtkRange* range, GtkScrollType, gdouble, gpointer) {
    sim.update_speed(gtk_range_get_value(range));
}
