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

// template<typename C>
// using Middle = typename aw::Signal<C>::function;

GTKInterface::GTKInterface()
: application(gtk_application_new("ufsc.yasims", G_APPLICATION_FLAGS_NONE)) {
    aw::Signal<GTKInterface>::set_receiver(*this);
    g_signal_connect(
        application,
        "activate",
        G_CALLBACK(SigGTKInterface<void>::callback<&GTKInterface::activate>),
        nullptr
    );
}

void GTKInterface::activate() {
    builder = gtk_builder_new_from_file("../res/view_simpl.ui");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    auto raw_canvas = gtk_builder_get_object(builder, "animation_area");
    auto canvas = GTK_DRAWING_AREA(raw_canvas);

    gtk_application_add_window(application, GTK_WINDOW(window));

    g_signal_connect(
        window,
        "delete-event",
        G_CALLBACK(SigContext<void>::callback<&Context::close>),
        nullptr
    );
    
    connect_buttons();
    
    g_signal_connect(
        canvas,
        "configure-event",
        (GCallback)(SigCairo<bool, GtkWidget*, GdkEventConfigure*, gpointer>
            ::callback<&Cairo::update>),
        nullptr
    );

    gtk_widget_show_all(window);
}

void GTKInterface::connect_buttons() {
    auto config = GTK_BUTTON(gtk_builder_get_object(builder, "configure-button"));
    auto summary = GTK_BUTTON(gtk_builder_get_object(builder, "summary-button"));
    auto start = GTK_BUTTON(gtk_builder_get_object(builder, "start-button"));
    auto fast = GTK_BUTTON(gtk_builder_get_object(builder, "fast-forward-button"));
    auto step = GTK_BUTTON(gtk_builder_get_object(builder, "step-button"));
    auto stop = GTK_BUTTON(gtk_builder_get_object(builder, "stop-button"));
    auto pause = GTK_BUTTON(gtk_builder_get_object(builder, "pause-button"));

    g_signal_connect(
        config, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::configure>),
        nullptr
    );

    g_signal_connect(
        summary, "clicked",
        G_CALLBACK(SigContext<void>::callback<&Context::summary>),
        nullptr
    );

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
}

void GTKInterface::destroy() {
    g_application_quit(G_APPLICATION(application));
    g_object_unref(application);
    g_object_unref(window);
}

void GTKInterface::show_message(GtkMessageType type, GtkButtonsType buttons,
                             const std::string& title, const std::string& message,
                             const std::string& submessage) const {
    auto dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT, type, buttons, message.c_str());
    
    gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
                                             submessage.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
