
#include "interface/Configuration.hpp"

Configuration::Configuration(GtkWidget* dialog):
 dialog{dialog} { }

int Configuration::run() {
    gtk_widget_show_all(dialog);
    return gtk_dialog_run(GTK_DIALOG(dialog));
}
