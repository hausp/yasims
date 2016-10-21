
#ifndef SMAIL_CONFIG_HPP
#define SMAIL_CONFIG_HPP

#include "smail/Default.hpp"

namespace smail {
    struct Config {
        double sim_time = Default::SIM_TIME;
        double timeout = Default::TIME_OUT;
        std::array<size_t, 2> center_capacities = {
            Default::LOCAL_CENTER_CAPACITY,
            Default::REMOTE_CENTER_CAPACITY
        };
        std::array<dist::funct<>, 2> arrival_times = {
            Default::L_ARRIVAL_TIMES,
            Default::R_ARRIVAL_TIMES
        };
        std::array<dist::disc<Address>, 2> destinations = {
            Default::L_DESTINATION,
            Default::R_DESTINATION
        };
        std::array<AWMap, 2> status_weights = {
            Default::L_STATUS_WEIGHTS,
            Default::R_STATUS_WEIGHTS
        };
        MFMap reception_times = Default::RECEPTION_TIMES;
        std::array<MFMap, 2> processing_times = {
            Default::L_PROCESSING_TIMES,
            Default::R_PROCESSING_TIMES
        };

        void synchronize_seed();
    };
}

#endif /* SMAIL_CONFIG_HPP */
