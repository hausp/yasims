
#ifndef SMAIL_DEFAULT_HPP
#define SMAIL_DEFAULT_HPP

#include <queue>
#include <unordered_map>
#include "Message.hpp"
#include "random/Function.hpp"

namespace smail {
    using MsgQueue = std::priority_queue<Message, std::deque<Message>>;
    using Weights = std::initializer_list<double>;
    using AWMap = std::unordered_map<Address, Weights>;
    using MFMap = std::unordered_map<Message, dist::funct<>>;
    
    struct Default {
        static const dist::disc<Address> L_DESTINATION;
        static const dist::disc<Address> R_DESTINATION;

        static const AWMap L_STATUS_WEIGHTS;
        static const AWMap R_STATUS_WEIGHTS;

        static const dist::funct<> L_ARRIVAL_TIMES;
        static const dist::funct<> R_ARRIVAL_TIMES;
        
        static const MFMap L_PROCESSING_TIMES;
        static const MFMap R_PROCESSING_TIMES;
        
        static const MFMap L_RECEPTION_TIMES;
        static const MFMap R_RECEPTION_TIMES;

    };
}

#endif /* SMAIL_DEFAULT_HPP */
