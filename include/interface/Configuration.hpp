
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <array>
#include <gtk/gtk.h>
#include <string>
#include <unordered_map>

#include "smail/Parser.hpp"
#include "smail/Message.hpp"

class Configuration {
    using MEMap = std::unordered_map<smail::Message, GtkEntry*>;
    using MSMap = std::unordered_map<smail::Message, std::string>;
 public:
    enum class Instance { UNIQUE };

    Configuration() = default;
    Configuration(GtkBuilder*);

    int run();
    parser::RawConfig extract() const;
 private:
    GtkWidget* dialog;

    GtkEntry* seed;
    GtkEntry* sim_time;
    GtkEntry* timeout;
    std::array<GtkEntry*, 2> center_sizes;
    std::array<GtkEntry*, 2> generations;
    std::array<GtkEntry*, 2> local_proportions;
    std::array<GtkEntry*, 2> remote_proportions;
    MEMap local_weights;
    MEMap remote_weights;
    MEMap reception_times;
    MEMap local_processing_times;
    MEMap remote_processing_times;

    std::string seed_str;
    std::string sim_time_str;
    std::string timeout_str;
    std::array<std::string, 2> center_sizes_str;
    std::array<std::string, 2> generations_str;
    std::array<std::string, 2> local_proportions_str;
    std::array<std::string, 2> remote_proportions_str;
    MSMap local_weights_str;
    MSMap remote_weights_str;
    MSMap reception_times_str;
    MSMap local_processing_times_str;
    MSMap remote_processing_times_str;

    parser::RawConfig last_config;

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

    void backup_strings();
    void restore_strings();
    bool validate_input();
};

inline parser::RawConfig Configuration::extract() const {
    return last_config;
}

#endif /* CONFIGURATION_HPP */
