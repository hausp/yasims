/* created by Marleson Graf<aszdrick@gmail.com>
   and Vinicius Freitas<vinicius.mctf@grad.ufsc.br> [2016] */

#include "GTKInterface.hpp"
#include "core/signals.hpp"

GTKInterface::GTKInterface()
: application(gtk_application_new("ufsc.yasims", G_APPLICATION_FLAGS_NONE)) {
    g_signal_connect(
        application,
        "activate",
        G_CALLBACK(signals::activate),
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
        G_CALLBACK(signals::close),
        nullptr
    );
    
    connect_buttons();

    
    g_signal_connect(
        canvas,
        "configure-event",
        G_CALLBACK(signals::configure_animation),
        nullptr
    );

    gtk_widget_show_all(window);
}

void GTKInterface::connect_buttons() {
    auto pause = GTK_BUTTON(gtk_builder_get_object(builder, "pause_button"));
    auto start = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
    auto stop = GTK_BUTTON(gtk_builder_get_object(builder, "stop_button"));

    g_signal_connect(pause, "clicked", G_CALLBACK(signals::pause), nullptr);
    g_signal_connect(start, "clicked", G_CALLBACK(signals::start), nullptr);
    g_signal_connect(stop, "clicked", G_CALLBACK(signals::stop), nullptr);
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
