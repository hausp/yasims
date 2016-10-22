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
    if (gui.show_configuration_dialog()) {
        auto raw = gui.raw_config();
        sim.update_config(parser::lapidate(std::move(raw)));
    }
}

void Context::summary() {
    sim.generate_output("../output.txt");
}

void Context::start() {
    sim.start(true);
    gui.set_active_start(false);
    gui.set_active_fast_forward(false);
    gui.set_active_step(false);
    gui.set_active_pause(true);
    gui.set_active_stop(true);
    gui.set_active_configuration(false);
    gui.set_active_summary(false);
}

void Context::fast_forward() {
    sim.start(false);
    gui.set_active_start(false);
    gui.set_active_fast_forward(false);
    gui.set_active_step(false);
    gui.set_active_pause(true);
    gui.set_active_stop(true);
    gui.set_active_configuration(false);
    gui.set_active_summary(false);
}

void Context::step() {
    sim.step();
    gui.set_active_stop(true);
    gui.set_active_configuration(false);
    gui.set_active_summary(false);
}

void Context::pause() {
    sim.pause();
    gui.set_active_start(true);
    gui.set_active_fast_forward(true);
    gui.set_active_step(true);
    gui.set_active_pause(false);
    gui.set_active_stop(true);
    gui.set_active_configuration(false);
    gui.set_active_summary(false);
}

void Context::stop() {
    sim.stop();
    gui.set_active_start(true);
    gui.set_active_fast_forward(true);
    gui.set_active_step(true);
    gui.set_active_pause(false);
    gui.set_active_stop(false);
    gui.set_active_configuration(true);
    gui.set_active_summary(true);
}

bool Context::stop_gui_behaviour() {
    stop();
    return false;
}

void Context::close() {
    gui.destroy();
}

void Context::change_speed(GtkRange* range, GtkScrollType, gdouble, gpointer) {
    sim.update_speed(gtk_range_get_value(range));
}
