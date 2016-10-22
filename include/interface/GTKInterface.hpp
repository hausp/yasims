/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef GTK_INTERFACE_HPP
#define GTK_INTERFACE_HPP

#include <gtk/gtk.h>
#include <memory>
#include <utility>

#include "Configuration.hpp"

class GTKInterface {
 public:
    enum class Instance { UNIQUE };

    GTKInterface();

    void activate();
    void destroy();

    parser::RawConfig raw_config();    
    void set_active_start(bool);
    void set_active_fast_forward(bool);
    void set_active_step(bool);
    void set_active_pause(bool);
    void set_active_stop(bool);
    void set_active_configuration(bool);
    void set_active_summary(bool);

    void show_statistics(std::array<std::string, 44>);
    int show_configuration_dialog();
    void error_message(GtkWidget* parent);
    void show_message(GtkWidget*, GtkMessageType,
                      GtkButtonsType, const std::string&,
                      const std::string&, const std::string&) const;

    GApplication* operator()();
 private:
    GtkApplication* application;
    GtkBuilder* builder;
    GtkWidget* window;
    GtkWidget* start;
    GtkWidget* fast;
    GtkWidget* step;
    GtkWidget* pause;
    GtkWidget* stop;
    GtkWidget* config;
    GtkWidget* summary;
    std::array<GtkLabel*, 44> statistics_entries;

    Configuration configuration;

    void connect_buttons();
    void init_statistics_entries();
};

inline GApplication* GTKInterface::operator()() {
    return G_APPLICATION(application);
}

#endif /* GTK_INTERFACE_HPP */
