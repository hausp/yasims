
#include "interface/Configuration.hpp"

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
    seed = GTK_WIDGET(gtk_builder_get_object(builder, "seed-entry"));
    
    sim_time = GTK_WIDGET(gtk_builder_get_object(builder, "sim-time-entry"));
    timeout = GTK_WIDGET(gtk_builder_get_object(builder, "timeout-entry"));
    
    center_sizes = {
        GTK_WIDGET(gtk_builder_get_object(builder, "num-servers-lc-entry")),
        GTK_WIDGET(gtk_builder_get_object(builder, "num-servers-rc-entry"))
    };
    // Second tab attributes
    generations = {
        GTK_WIDGET(gtk_builder_get_object(builder, "local-gen-entry")),
        GTK_WIDGET(gtk_builder_get_object(builder, "remote-gen-entry"))
    };
    
    local_proportions = {
        GTK_WIDGET(gtk_builder_get_object(builder, "ll-percent-entry")),
        GTK_WIDGET(gtk_builder_get_object(builder, "lr-percent-entry"))
    };
        
    remote_proportions = {
        GTK_WIDGET(gtk_builder_get_object(builder, "rl-percent-entry")),
        GTK_WIDGET(gtk_builder_get_object(builder, "rr-percent-entry"))
    };

    local_weights = {
        {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "lls-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "llf-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "llp-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrs-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrf-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrp-entry"))}
    };

    remote_weights = {
        {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "rls-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "rlf-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "rlp-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrs-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrf-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrp-entry"))}
    };
    // Third tab attributes
    reception_times = {
        {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "lls-rt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "llf-rt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "llp-rt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrs-rt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrf-rt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrp-rt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "rls-rt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "rlf-rt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "rlp-rt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrs-rt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrf-rt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrp-rt-entry"))}
    };

    local_processing_times = {
        {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "lls-pt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "llf-pt-entry"))},
        {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "llp-pt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrs-pt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrf-pt-entry"))},
        {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "lrp-pt-entry"))}
    };

    remote_processing_times = {
        {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "rls-pt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "rlf-pt-entry"))},
        {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "rlp-pt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrs-pt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrf-pt-entry"))},
        {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
          GTK_WIDGET(gtk_builder_get_object(builder, "rrp-pt-entry"))}
    };

    // set_defaults();
}

int Configuration::run() {
    // Backup strings in case the user do shit
    backup_strings();
    // Show the treasure
    gtk_widget_show_all(dialog);
    // While he things and do shit, stay still
    auto dumb_user_thinking = true;
    // Response: 0 = cancel, 1 = ok
    auto response = 0;
    while(dumb_user_thinking) {
        response = gtk_dialog_run(GTK_DIALOG(dialog));
        dumb_user_thinking = response;
        if (!response) {
            restore_strings();
            dumb_user_thinking = false;
        } else {
            dumb_user_thinking = validate_input();
        }
    }
    gtk_widget_hide(dialog);
    return response;
}

void Configuration::backup_strings() {
    for (auto i = 0; i < 2; i++) {
        center_sizes_str[i] = gtk_entry_get_text(GTK_ENTRY(center_sizes[i]));
        generations_str[i] = gtk_entry_get_text(GTK_ENTRY(generations[i]));    
        local_proportions_str[i] = gtk_entry_get_text(
            GTK_ENTRY(local_proportions[i]));
        remote_proportions_str[i] = gtk_entry_get_text(
            GTK_ENTRY(remote_proportions[i]));
    }

    for (auto pair : local_weights) {
        local_weights_str[pair.first] =
            gtk_entry_get_text(GTK_ENTRY(pair.second));
    }

    for (auto pair : remote_weights) {
        remote_weights_str[pair.first] =
            gtk_entry_get_text(GTK_ENTRY(pair.second));
    }

    for (auto pair : reception_times) {
        reception_times_str[pair.first] =
            gtk_entry_get_text(GTK_ENTRY(pair.second));
    }

    for (auto pair : local_processing_times) {
        local_processing_times_str[pair.first] =
            gtk_entry_get_text(GTK_ENTRY(pair.second));
    }

    for (auto pair : remote_processing_times) {
        remote_processing_times_str[pair.first] =
            gtk_entry_get_text(GTK_ENTRY(pair.second));
    }
}

bool Configuration::validate_input() {

    return true;
}

void Configuration::restore_strings() {
    for (auto i = 0; i < 2; i++) {
        gtk_entry_set_text(
            GTK_ENTRY(center_sizes[i]), center_sizes_str[i].c_str());
        gtk_entry_set_text(
            GTK_ENTRY(generations[i]), generations_str[i].c_str());
        gtk_entry_set_text(
            GTK_ENTRY(local_proportions[i]),
            local_proportions_str[i].c_str());
        gtk_entry_set_text(
            GTK_ENTRY(remote_proportions[i]),
            remote_proportions_str[i].c_str());
    }

    for (auto pair : local_weights) {
        gtk_entry_set_text(
            GTK_ENTRY(pair.second),
            local_weights_str[pair.first].c_str());
    }

    for (auto pair : remote_weights) {
        gtk_entry_set_text(
            GTK_ENTRY(pair.second),
            remote_weights_str[pair.first].c_str());
    }

    for (auto pair : reception_times) {
        gtk_entry_set_text(
            GTK_ENTRY(pair.second),
            reception_times_str[pair.first].c_str());
    }

    for (auto pair : local_processing_times) {
        gtk_entry_set_text(
            GTK_ENTRY(pair.second),
            local_processing_times_str[pair.first].c_str());
    }

    for (auto pair : remote_processing_times) {
        gtk_entry_set_text(
            GTK_ENTRY(pair.second),
            remote_processing_times_str[pair.first].c_str());
    }
}
