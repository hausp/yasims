
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

        static std::random_device rd;

        static const AWMap local_status_weights;
        static const AWMap remote_status_weights;

        static const dist::funct<> local_arrival_time;
        static const dist::funct<> remote_arrival_time;
        
        static const MFMap local_reception_times;
        static const MFMap remote_reception_times;

        static const MFMap local_processing_times;
        static const MFMap remote_processing_times;
    };
}

#endif /* SMAIL_DEFAULT_HPP */
