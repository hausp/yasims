/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef GTK_INTERFACE_HPP
#define GTK_INTERFACE_HPP

#include <gtk/gtk.h>
#include <memory>
#include <utility>
#include <string>

struct RawConfig {
    std::array<std::string, 2> local_traffic;
    std::array<std::string, 2> remote_traffic;

};

class GTKInterface {
 public:
    enum class Instance { UNIQUE };

    GTKInterface();

    void activate();
    void destroy();
    
    RawConfig show_configuration_window();
    void show_message(GtkMessageType, GtkButtonsType, const std::string&,
                 const std::string&, const std::string&) const;

    GApplication* operator()();
 private:
    GtkApplication* application;
    GtkBuilder* builder;
    GtkWidget* config_window;
    GtkWidget* window;

    void connect_buttons();
};

inline GApplication* GTKInterface::operator()() {
    return G_APPLICATION(application);
}

#endif /* GTK_INTERFACE_HPP */
