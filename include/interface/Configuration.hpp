
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
    using MSMap = std::unordered_map<smail::Message, std::string>;
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

    std::array<std::string, 2> center_sizes_str;
    std::array<std::string, 2> generations_str;
    std::array<std::string, 2> local_proportions_str;
    std::array<std::string, 2> remote_proportions_str;
    MSMap local_weights_str;
    MSMap remote_weights_str;
    MSMap reception_times_str;
    MSMap local_processing_times_str;
    MSMap remote_processing_times_str;

    RawConfig last_config;

    static const smail::Message llsm;
    static const smail::Message llfm;
    static const smail::Message llpm;
    static const smail::Message lrsm;
    static const smail::Message lrfm;
    static const smail::Message lrpm;
    static const smail::Message rlsm;
    static const smail::Message rlfm;
    static const smail::Message rlpm;
    static const smail::Message rrsm;
    static const smail::Message rrfm;
    static const smail::Message rrpm;

    // void set_defaults();
    void backup_strings();
    void restore_strings();
    bool validate_input();
};

inline RawConfig Configuration::extract() const {
    return last_config;
}

#endif /* CONFIGURATION_HPP */
