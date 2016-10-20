
#ifndef SMAIL_TYPES_HPP
#define SMAIL_TYPES_HPP

#include <unordered_map>
#include "random/Function.hpp"

namespace smail {
    struct Message;
    struct Event;
    struct Counters {
        unsigned messages_in_system;
        unsigned local_local_messages;
        unsigned local_remote_messages;
        unsigned remote_local_messages;
        unsigned remote_remote_messages;
        unsigned total_finished_messages;
        double mean_message_time_in_system;        
    };

    enum class Address : unsigned {
        LOCAL = 0,
        REMOTE = 1,
    };

    enum class Status : unsigned {
        SUCCESS = 0,
        FAILURE = 1,
        POSTPONED = 2,
        UNDEFINED = 3,
    };

    using MsgQueue = std::priority_queue<Message,
        std::deque<Message>, std::greater<Message>>;
    using Weights = std::initializer_list<double>;
    using AWMap = std::unordered_map<Address, Weights>;
    using MFMap = std::unordered_map<Message, dist::funct<>>;
}

#endif /* SMAIL_TYPES_HPP */
