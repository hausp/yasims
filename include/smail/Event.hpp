
#ifndef SMAIL_EVENT_HPP
#define SMAIL_EVENT_HPP

#include "Message.hpp"

namespace smail {
    struct Event {
        using Action = std::function<void(double)>;

        double time;
        Message message;
        Action pre_action;
        Action pos_action;
    };

    inline bool operator<(const Event& lhs, const Event& rhs) {
        return lhs.time < rhs.time;
    }
}

#endif /* SMAIL_EVENT_HPP */
