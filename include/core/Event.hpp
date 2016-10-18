// 

#ifndef EVENT_HPP
#define EVENT_HPP

#include "Message.hpp"

struct Event {
    using Action = std::function<void(const Message&, double)>;

    double time;
    Message message;
    Action pre_action;
    Action pos_action;
};

inline bool operator<(const Event& lhs, const Event& rhs) {
    return lhs.time < rhs.time;
}

#endif /* EVENT_HPP */
