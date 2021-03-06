
#ifndef SMAIL_DEFAULT_HPP
#define SMAIL_DEFAULT_HPP

#include <queue>
#include <unordered_map>
#include "Message.hpp"
#include "random/Function.hpp"
#include "Types.hpp"

namespace smail {
    struct Default {
        static constexpr size_t SIM_TIME = 6000;
        static constexpr size_t SPEED_NUMERATOR = 1000;
        static constexpr size_t SPEED_DENOMINATOR = 30;
        static constexpr double TIME_OUT = 0.3;
        static constexpr size_t LOCAL_CENTER_CAPACITY = 10;
        static constexpr size_t REMOTE_CENTER_CAPACITY = 20;

        static const dist::disc<Address> L_DESTINATION;
        static const dist::disc<Address> R_DESTINATION;

        static const AWMap L_STATUS_WEIGHTS;
        static const AWMap R_STATUS_WEIGHTS;

        static const dist::funct<> L_ARRIVAL_TIMES;
        static const dist::funct<> R_ARRIVAL_TIMES;
        
        static const MFMap RECEPTION_TIMES;

        static const MFMap L_PROCESSING_TIMES;
        static const MFMap R_PROCESSING_TIMES;
    };
}

#endif /* SMAIL_DEFAULT_HPP */
