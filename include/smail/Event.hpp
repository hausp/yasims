
#ifndef SMAIL_EVENT_HPP
#define SMAIL_EVENT_HPP

#include "Message.hpp"

namespace smail {
    struct Event {
        using Action = std::function<void()>;

        double time;
        Action action;
    };

    inline bool operator<(const Event& lhs, const Event& rhs) {
        return lhs.time < rhs.time;
    }
}

#endif /* SMAIL_EVENT_HPP */
