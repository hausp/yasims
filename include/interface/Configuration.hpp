
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <array>
#include <gtk/gtk.h>
#include <string>
#include <unordered_map>

#include "smail/Parser.hpp"
#include "smail/Message.hpp"

struct RawConfig {
    std::array<std::string, 2> local_traffic;
    std::array<std::string, 2> remote_traffic;
    // TODO
};

class Configuration {
    using MGMap = std::unordered_map<smail::Message, GtkWidget*>;
 public:
    enum class Instance { UNIQUE };

    Configuration() = default;
    Configuration(GtkBuilder*);

    int run();
    RawConfig extract() const;
 private:
    GtkWidget* dialog;
    GtkWidget* seed;
    GtkWidget* sim_time;
    GtkWidget* timeout;
    std::array<GtkWidget*, 2> center_sizes;
    std::array<GtkWidget*, 2> generations;
    std::array<GtkWidget*, 2> local_proportions;
    std::array<GtkWidget*, 2> remote_proportions;
    MGMap local_weights;
    MGMap remote_weights;
    MGMap reception_times;
    MGMap local_processing_times;
    MGMap remote_processing_times;

    RawConfig last_config;

    void set_defaults();
};

inline RawConfig Configuration::extract() const {
    return last_config;
}

#endif /* CONFIGURATION_HPP */
