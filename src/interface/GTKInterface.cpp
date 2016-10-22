/* created by Marleson Graf<aszdrick@gmail.com>
   and Vinicius Freitas<vinicius.mctf@grad.ufsc.br> [2016] */

#include "interface/GTKInterface.hpp"
#include "core/Context.hpp"
#include "wrapper/Cairo.hpp"
#include "wrapper/Signal.hpp"
#include <iostream>

template<typename R, typename... Args>
using SigContext = aw::Signal<Context>::function<R(Args...)>;

template<typename R, typename... Args>
using SigCairo = aw::Signal<Cairo>::function<R(Args...)>;

template<typename R, typename... Args>
using SigGTKInterface = aw::Signal<GTKInterface>::function<R(Args...)>;

GTKInterface::GTKInterface()
: application{
    gtk_application_new("ufsc.yasims", G_APPLICATION_FLAGS_NONE)} {
    aw::Signal<GTKInterface>::set_receiver(*this);
    g_signal_connect(
        application,
        "activate",
        G_CALLBACK(SigGTKInterface<void>
            ::callback<&GTKInterface::activate>),
        nullptr
    );
}

void GTKInterface::activate() {
    builder = gtk_builder_new_from_file("../res/view_simpl.ui");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    configuration = Configuration{builder};

    // gtk_window_set_resizable(GTK_WINDOW(window), false);
    gtk_application_add_window(application, GTK_WINDOW(window));

    g_signal_connect(
        window,
        "delete-event",
        G_CALLBACK(SigContext<void>::callback<&Context::close>),
        nullptr
    );
    
    init_statistics_entries();
    connect_buttons();

    g_signal_connect(
        application,
        "activate",
        G_CALLBACK(SigGTKInterface<void>
            ::callback<&GTKInterface::activate>),
        nullptr
    );

    gtk_widget_show_all(window);
}

void GTKInterface::set_active_start(bool active) {
    gtk_widget_set_sensitive(start, active);
}

void GTKInterface::set_active_fast_forward(bool active) {
    gtk_widget_set_sensitive(fast, active);
}

void GTKInterface::set_active_step(bool active) {
    gtk_widget_set_sensitive(step, active);
}

void GTKInterface::set_active_pause(bool active) {
    gtk_widget_set_sensitive(pause, active);
}

void GTKInterface::set_active_stop(bool active) {
    gtk_widget_set_sensitive(stop, active);
}

void GTKInterface::set_active_configuration(bool active) {
    gtk_widget_set_sensitive(config, active);
}

void GTKInterface::set_active_summary(bool active) {
    gtk_widget_set_sensitive(summary, active);
}

void GTKInterface::init_statistics_entries() {
    statistics_entries = {{
        GTK_LABEL(gtk_builder_get_object(builder, "clock-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "sys-occupation-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "min-occupation-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "max-occupation-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "mean-occupation-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "total-msg-arrived-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "ll-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lr-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rl-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rr-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lls-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "llf-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "llp-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lrs-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lrf-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lrp-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rls-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rlf-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rlp-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rrs-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rrf-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rrp-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "q-size-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "q-avg-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lproc-q-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lproc-q-avg-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lproc-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lproc-avg-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rproc-q-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rproc-q-avg-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rproc-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "rproc-avg-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "lls-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "llf-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "lrs-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "lrf-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "rls-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "rlf-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "rrs-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "rrf-count-label1")),
        GTK_LABEL(gtk_builder_get_object(builder, "total-exited-count-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "min-msg-time-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "max-msg-time-label")),
        GTK_LABEL(gtk_builder_get_object(builder, "avg-msg-time-label"))
    }};
}

void GTKInterface::connect_buttons() {
    config = GTK_WIDGET(gtk_builder_get_object(builder, "configure-button"));
    // summary = GTK_WIDGET(gtk_builder_get_object(builder, "summary-button"));
    start = GTK_WIDGET(gtk_builder_get_object(builder, "start-button"));
    fast = GTK_WIDGET(gtk_builder_get_object(builder, "fast-forward-button"));
    step = GTK_WIDGET(gtk_builder_get_object(builder, "step-button"));
    stop = GTK_WIDGET(gtk_builder_get_object(builder, "stop-button"));
    pause = GTK_WIDGET(gtk_builder_get_object(builder, "pause-button"));
    auto speed = GTK_WIDGET(gtk_builder_get_object(builder, "speed-scale"));

    gtk_widget_set_sensitive(stop, false);
    gtk_widget_set_sensitive(pause, false);

    g_signal_connect(
        config, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::configure>),
        nullptr
    );

    // g_signal_connect(
    //     summary, "clicked",
    //     G_CALLBACK(SigContext<void>::callback<&Context::summary>),
    //     nullptr
    // );

    g_signal_connect(
        start, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::start>),
        nullptr
    );

    g_signal_connect(
        fast, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::fast_forward>),
        nullptr
    );

    g_signal_connect(
        step, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::step>),
        nullptr
    );

    g_signal_connect(
        pause, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::pause>),
        nullptr
    );

    g_signal_connect(
        stop, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::stop>),
        nullptr
    );

    g_signal_connect(
        speed, "value-changed",
        (GCallback)(SigContext<void, GtkRange*, GtkScrollType, gdouble, gpointer>
            ::callback<&Context::change_speed>),
        nullptr
    );
}

void GTKInterface::destroy() {
    g_application_quit(G_APPLICATION(application));
    g_object_unref(application);
    g_object_unref(window);
}


int GTKInterface::show_configuration_dialog() {
    return configuration.run();
}

parser::RawConfig GTKInterface::raw_config() {
    return configuration.extract();
}

void GTKInterface::show_statistics(std::array<std::string, 44> statistics) {
    for (auto i = 0; i < 43; i++) {
        auto text = statistics[i].c_str();
        gtk_label_set_text(statistics_entries[i], text);
    }
}

void GTKInterface::error_message(GtkWidget* parent) {
    show_message(
        parent,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "Erro!",
        "Valor desconhecido encontrado",
        "Por favor, verifique os campos preenchidos."
    );
}

void GTKInterface::show_message(GtkWidget* parent,
                                GtkMessageType type,
                                GtkButtonsType buttons,
                                const std::string& title,
                                const std::string& message,
                                const std::string& submessage) const {
    auto dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
        GTK_DIALOG_DESTROY_WITH_PARENT, type, buttons, message.c_str());
    
    gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
                                             submessage.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
