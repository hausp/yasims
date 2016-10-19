
#ifndef SMAIL_MESSAGE_HPP
#define SMAIL_MESSAGE_HPP

#include <functional>

namespace smail {
    enum class Address {
        LOCAL,
        REMOTE,
    };

    enum class Status {
        SUCCESS,
        FAILURE,
        POSTPONED,
    };

    struct Message {
        using Type = Address;

        Address from;
        Address to;
        Status status;
    };

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
