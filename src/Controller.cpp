/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "Controller.hpp"

Controller::Controller(Interface& ui)
 : ui(ui) { }

void Controller::close() {
    ui.destroy();
    gtk::quit();
}