
#include "interface/Configuration.hpp"

Configuration::Configuration(GtkBuilder* builder) {
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "config-dialog"));
    // gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancelar", 0);
    // gtk_dialog_add_button(GTK_DIALOG(dialog), "Confirmar", 1);
}

int Configuration::run() {
    gtk_widget_show_all(dialog);
    auto response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_hide(dialog);
    return response;
}

