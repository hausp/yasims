
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <array>
#include <gtk/gtk.h>
#include <string>

#include "smail/Parser.hpp"

struct RawConfig {
    std::array<std::string, 2> local_traffic;
    std::array<std::string, 2> remote_traffic;
    // TODO
};

class Configuration {
 public:
    Configuration() = default;
    Configuration(GtkWidget*);

    int run();
    RawConfig extract() const;
 private:
    GtkWidget* dialog;
    RawConfig last_config;
};

inline RawConfig Configuration::extract() const {
    return last_config;
}

#endif /* CONFIGURATION_HPP */
