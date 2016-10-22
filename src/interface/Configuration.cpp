
#include "interface/Configuration.hpp"
#include "interface/GTKInterface.hpp"
#include "wrapper/Signal.hpp"

const smail::Message Configuration::llsm = {
    smail::Address::LOCAL, smail::Address::LOCAL, smail::Status::SUCCESS};
const smail::Message Configuration::llfm = {
    smail::Address::LOCAL, smail::Address::LOCAL, smail::Status::FAILURE};
const smail::Message Configuration::llpm = {
    smail::Address::LOCAL, smail::Address::LOCAL, smail::Status::POSTPONED};
const smail::Message Configuration::lrsm = {
    smail::Address::LOCAL, smail::Address::REMOTE, smail::Status::SUCCESS};
const smail::Message Configuration::lrfm = {
    smail::Address::LOCAL, smail::Address::REMOTE, smail::Status::FAILURE};
const smail::Message Configuration::lrpm = {
    smail::Address::LOCAL, smail::Address::REMOTE, smail::Status::POSTPONED};
const smail::Message Configuration::rlsm = {
    smail::Address::REMOTE, smail::Address::LOCAL, smail::Status::SUCCESS};
const smail::Message Configuration::rlfm = {
    smail::Address::REMOTE, smail::Address::LOCAL, smail::Status::FAILURE};
const smail::Message Configuration::rlpm = {
    smail::Address::REMOTE, smail::Address::LOCAL, smail::Status::POSTPONED};
const smail::Message Configuration::rrsm = {
    smail::Address::REMOTE, smail::Address::REMOTE, smail::Status::SUCCESS};
const smail::Message Configuration::rrfm = {
    smail::Address::REMOTE, smail::Address::REMOTE, smail::Status::FAILURE};
const smail::Message Configuration::rrpm = {
    smail::Address::REMOTE, smail::Address::REMOTE, smail::Status::POSTPONED};


Configuration::Configuration(GtkBuilder* builder) {
    using namespace smail;
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "config-dialog"));
    // First tab attributes
    seed = GTK_ENTRY(gtk_builder_get_object(builder, "seed-entry"));
    
    sim_time = GTK_ENTRY(gtk_builder_get_object(builder, "sim-time-entry"));
    timeout = GTK_ENTRY(gtk_builder_get_object(builder, "timeout-entry"));
    
    center_sizes = {{
        GTK_ENTRY(gtk_builder_get_object(builder, "num-servers-lc-entry")),
        GTK_ENTRY(gtk_builder_get_object(builder, "num-servers-rc-entry"))
    }};
    // Second tab attributes
    generations = {{
        GTK_ENTRY(gtk_builder_get_object(builder, "local-gen-entry")),
        GTK_ENTRY(gtk_builder_get_object(builder, "remote-gen-entry"))
    }};
    
    local_proportions = {{
        GTK_ENTRY(gtk_builder_get_object(builder, "ll-percent-entry")),
        GTK_ENTRY(gtk_builder_get_object(builder, "lr-percent-entry"))
    }};
        
    remote_proportions = {{
        GTK_ENTRY(gtk_builder_get_object(builder, "rl-percent-entry")),
        GTK_ENTRY(gtk_builder_get_object(builder, "rr-percent-entry"))
    }};

    local_weights = {
        {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "lls-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "llf-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "llp-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrs-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrf-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrp-entry"))}
    };

    remote_weights = {
        {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "rls-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "rlf-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "rlp-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrs-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrf-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrp-entry"))}
    };
    // Third tab attributes
    reception_times = {
        {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "lls-rt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "llf-rt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "llp-rt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrs-rt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrf-rt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrp-rt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "rls-rt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "rlf-rt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "rlp-rt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrs-rt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrf-rt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrp-rt-entry"))}
    };

    local_processing_times = {
        {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "lls-pt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "llf-pt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "llp-pt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrs-pt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrf-pt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "lrp-pt-entry"))}
    };

    remote_processing_times = {
        {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "rls-pt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "rlf-pt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "rlp-pt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrs-pt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrf-pt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
          GTK_ENTRY(gtk_builder_get_object(builder, "rrp-pt-entry"))}
    };
}

int Configuration::run() {
    backup_strings();
    gtk_widget_show_all(dialog);
    auto thinking = true;
    auto response = 0;
    while(thinking) {
        response = std::max(gtk_dialog_run(GTK_DIALOG(dialog)), 0);
        thinking = response && !validate_input();
        if (thinking) {
            if (response) {
                aw::Signal<GTKInterface>::function<void(GtkWidget*)>
                    ::callback<&GTKInterface::error_message>(dialog);
            } else {
                restore_strings();                
            }
        }
    }
    gtk_widget_hide(dialog);
    return response;
}

bool Configuration::validate_input() {
    auto match = parser::Match{};
    auto valid = false;
    
    std::tie(match, valid) = parser::s_match(
        gtk_entry_get_text(seed), "random");
    if (!valid || match.type != dist::Type::CONS) return false;
    last_config.seed = std::move(match);
    std::tie(match, valid) = parser::s_match(
        gtk_entry_get_text(sim_time), "infinite");
    if (!valid || match.type != dist::Type::CONS) return false;
    last_config.sim_time = std::move(match);
    std::tie(match, valid) = parser::match(gtk_entry_get_text(timeout));
    if (!valid || match.type != dist::Type::CONS) return false;
    last_config.timeout = std::move(match);
    for (size_t i = 0; i < center_sizes.size(); i++) {
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(center_sizes[i]));
        if (!valid) return false;
        last_config.center_sizes[i] = std::move(match);
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(generations[i]));
        if (!valid) return false;
        last_config.generations[i] = std::move(match);
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(local_proportions[i]));
        if (!valid) return false;
        last_config.local_proportions[i] = std::move(match);
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(remote_proportions[i]));
        if (!valid) return false;
        last_config.remote_proportions[i] = std::move(match);
    }
    auto i = 0;
    for (auto pair : local_weights) {
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(pair.second));
        if (!valid) return false;
        auto t = static_cast<size_t>(pair.first.to);
        last_config.local_weights[t][i] = std::move(match);
        i = (i + 1) % 3;
    }
    for (auto pair : remote_weights) {
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(pair.second));
        if (!valid) return false;
        auto t = static_cast<size_t>(pair.first.to);
        last_config.remote_weights[t][i] = std::move(match);
        i = (i + 1) % 3;
    }
    for (auto pair : reception_times) {
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(pair.second));
        if (!valid) return false;
        last_config.reception_times[pair.first] = std::move(match);
    }
    for (auto pair : local_processing_times) {
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(pair.second));
        if (!valid) return false;
        last_config.local_processing_times[pair.first] = std::move(match);
    }
    for (auto pair : remote_processing_times) {
        std::tie(match, valid) = 
            parser::match(gtk_entry_get_text(pair.second));
        if (!valid) return false;
        last_config.remote_processing_times[pair.first] = std::move(match);
    }
    return true;
}

void Configuration::backup_strings() {
    seed_str = gtk_entry_get_text(seed);
    sim_time_str = gtk_entry_get_text(sim_time);
    timeout_str = gtk_entry_get_text(timeout);

    for (size_t i = 0; i < 2; i++) {
        center_sizes_str[i] = gtk_entry_get_text(center_sizes[i]);
        generations_str[i] = gtk_entry_get_text(generations[i]);    
        local_proportions_str[i] = gtk_entry_get_text(local_proportions[i]);
        remote_proportions_str[i] = gtk_entry_get_text(remote_proportions[i]);
    }

    for (auto pair : local_weights) {
        local_weights_str[pair.first] = gtk_entry_get_text(pair.second);
    }

    for (auto pair : remote_weights) {
        remote_weights_str[pair.first] = gtk_entry_get_text(pair.second);
    }

    for (auto pair : reception_times) {
        reception_times_str[pair.first] = gtk_entry_get_text(pair.second);
    }

    for (auto pair : local_processing_times) {
        local_processing_times_str[pair.first] = gtk_entry_get_text(pair.second);
    }

    for (auto pair : remote_processing_times) {
        remote_processing_times_str[pair.first] = gtk_entry_get_text(pair.second);
    }
}

void Configuration::restore_strings() {
    gtk_entry_set_text(seed, seed_str.c_str());
    gtk_entry_set_text(sim_time, sim_time_str.c_str());
    gtk_entry_set_text(timeout, timeout_str.c_str());

    for (size_t i = 0; i < center_sizes.size(); i++) {
        gtk_entry_set_text(center_sizes[i], center_sizes_str[i].c_str());
        gtk_entry_set_text(generations[i], generations_str[i].c_str());
        gtk_entry_set_text(local_proportions[i], local_proportions_str[i].c_str());
        gtk_entry_set_text(remote_proportions[i], remote_proportions_str[i].c_str());
    }

    for (auto pair : local_weights) {
        gtk_entry_set_text(pair.second, local_weights_str[pair.first].c_str());
    }

    for (auto pair : remote_weights) {
        gtk_entry_set_text(pair.second, remote_weights_str[pair.first].c_str());
    }

    for (auto pair : reception_times) {
        gtk_entry_set_text(pair.second, reception_times_str[pair.first].c_str());
    }

    for (auto pair : local_processing_times) {
        gtk_entry_set_text(pair.second, local_processing_times_str[pair.first].c_str());
    }

    for (auto pair : remote_processing_times) {
        gtk_entry_set_text(pair.second, remote_processing_times_str[pair.first].c_str());
    }
}
