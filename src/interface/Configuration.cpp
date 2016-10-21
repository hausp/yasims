
#include "interface/Configuration.hpp"

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
          GTK_WIDGET(gtk_builder_get_object(builder, "lrp-entry"))},
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

    local_processing_times = {
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

    local_processing_times = {
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

    set_defaults();
}

int Configuration::run() {
    gtk_widget_show_all(dialog);
    auto response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_hide(dialog);
    return response;
}

void Configuration::set_defaults() {
    using namespace smail;
    const auto llsm = Message{Address::LOCAL, Address::LOCAL, Status::SUCCESS};
    const auto llfm = Message{Address::LOCAL, Address::LOCAL, Status::FAILURE};
    const auto llpm = Message{Address::LOCAL, Address::LOCAL, Status::POSTPONED};
    const auto lrsm = Message{Address::LOCAL, Address::REMOTE, Status::SUCCESS};
    const auto lrfm = Message{Address::LOCAL, Address::REMOTE, Status::FAILURE};
    const auto lrpm = Message{Address::LOCAL, Address::REMOTE, Status::POSTPONED};
    const auto rlsm = Message{Address::REMOTE, Address::LOCAL, Status::SUCCESS};
    const auto rlfm = Message{Address::REMOTE, Address::LOCAL, Status::FAILURE};
    const auto rlpm = Message{Address::REMOTE, Address::LOCAL, Status::POSTPONED};
    const auto rrsm = Message{Address::REMOTE, Address::REMOTE, Status::SUCCESS};
    const auto rrfm = Message{Address::REMOTE, Address::REMOTE, Status::FAILURE};
    const auto rrpm = Message{Address::REMOTE, Address::REMOTE, Status::POSTPONED};

    gtk_entry_set_text(GTK_ENTRY(seed), "random");
    gtk_entry_set_text(GTK_ENTRY(sim_time), "infinite");
    gtk_entry_set_text(GTK_ENTRY(timeout), "0.3");

    gtk_entry_set_text(GTK_ENTRY(center_sizes[0]), "20");
    gtk_entry_set_text(GTK_ENTRY(center_sizes[1]), "20");
    
    gtk_entry_set_text(GTK_ENTRY(generations[0]), "EXPO(0.5)");
    gtk_entry_set_text(GTK_ENTRY(generations[1]), "EXPO(0.6)");
    
    gtk_entry_set_text(GTK_ENTRY(local_proportions[0]), "0.667");
    gtk_entry_set_text(GTK_ENTRY(local_proportions[1]), "0.333");
    gtk_entry_set_text(GTK_ENTRY(remote_proportions[0]), "0.60");
    gtk_entry_set_text(GTK_ENTRY(remote_proportions[1]), "0.40");


    gtk_entry_set_text(GTK_ENTRY(local_weights[llsm]), "0.87");
    gtk_entry_set_text(GTK_ENTRY(local_weights[llfm]), "0.005");
    gtk_entry_set_text(GTK_ENTRY(local_weights[llpm]), "0.125");
    gtk_entry_set_text(GTK_ENTRY(local_weights[lrsm]), "0.96");
    gtk_entry_set_text(GTK_ENTRY(local_weights[lrfm]), "0.015");
    gtk_entry_set_text(GTK_ENTRY(local_weights[lrpm]), "0.025");

    gtk_entry_set_text(GTK_ENTRY(remote_weights[rlsm]), "0.96");
    gtk_entry_set_text(GTK_ENTRY(remote_weights[rlfm]), "0.03");
    gtk_entry_set_text(GTK_ENTRY(remote_weights[rlpm]), "0.01");
    gtk_entry_set_text(GTK_ENTRY(remote_weights[rrsm]), "0.90");
    gtk_entry_set_text(GTK_ENTRY(remote_weights[rrfm]), "0.01");
    gtk_entry_set_text(GTK_ENTRY(remote_weights[rrpm]), "0.09");

    gtk_entry_set_text(GTK_ENTRY(reception_times[llsm]), "0.12");
    gtk_entry_set_text(GTK_ENTRY(reception_times[llfm]), "0.14");
    gtk_entry_set_text(GTK_ENTRY(reception_times[llpm]), "0.11");
    gtk_entry_set_text(GTK_ENTRY(reception_times[lrsm]), "0.12");
    gtk_entry_set_text(GTK_ENTRY(reception_times[lrfm]), "0.13");
    gtk_entry_set_text(GTK_ENTRY(reception_times[lrpm]), "0.15");
    gtk_entry_set_text(GTK_ENTRY(reception_times[rlsm]), "0.12");
    gtk_entry_set_text(GTK_ENTRY(reception_times[rlfm]), "0.14");
    gtk_entry_set_text(GTK_ENTRY(reception_times[rlpm]), "0.11");
    gtk_entry_set_text(GTK_ENTRY(reception_times[rrsm]), "0.16");
    gtk_entry_set_text(GTK_ENTRY(reception_times[rrfm]), "0.13");
    gtk_entry_set_text(GTK_ENTRY(reception_times[rrpm]), "0.16");
}
