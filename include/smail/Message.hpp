
#ifndef SMAIL_MESSAGE_HPP
#define SMAIL_MESSAGE_HPP

#include <functional>
#include "Types.hpp"

namespace smail {
    struct Message {
        using Type = Address;

        Address from;
        Address to;
        Status status;

        double current_time = 0;
        double in_system_time = 0;
    };


    inline bool operator>(const Message& lhs, const Message& rhs) {
        return lhs.current_time > rhs.current_time;
    }

    inline bool operator==(const Message& lhs, const Message& rhs) {
        return lhs.from == rhs.from 
            && lhs.from == rhs.from 
            && lhs.status == lhs.status;
    }
}

namespace std {
    template<>
    struct hash<smail::Message> {
        inline size_t operator()(const smail::Message& m) const {
            auto f = static_cast<int>(m.from);
            auto t = static_cast<int>(m.to);
            auto s = static_cast<int>(m.status);
            return hash<int>()(f * 2 + t * 3 + s * 7);
        }
    };
}

#endif /* SMAIL_MESSAGE_HPP */
