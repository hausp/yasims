/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <utility>

#include "gtk.hpp"

class Interface {
 public:
    Interface();
    void show_message(GtkMessageType, GtkButtonsType, const std::string&,
                 const std::string&, const std::string&) const;
    void show();
    void destroy();

 private:
    GtkBuilder* builder;
    GtkWidget* window;

    void build();
    void connect_buttons();
};

#endif /* INTERFACE_HPP */
