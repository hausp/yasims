
#ifndef SMAIL_TYPES_HPP
#define SMAIL_TYPES_HPP

#include <unordered_map>
#include <queue>
#include "random/Function.hpp"

namespace smail {
    struct Message;
    struct Event;

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

    inline std::string to_string(const Address& addr) {
        switch(addr) {
            case Address::LOCAL:
                return "local";
            case Address::REMOTE:
                return "remote";
            default:
                return "";
        }
    }

    inline std::string to_string(const Status& st) {
        switch(st) {
            case Status::SUCCESS:
                return "success";
            case Status::FAILURE:
                return "failure";
            case Status::POSTPONED:
                return "postpone";
            default:
                return "";
        }
    }
}

#endif /* SMAIL_TYPES_HPP */
