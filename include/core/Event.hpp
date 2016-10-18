// 

#ifndef EVENT_HPP
#define EVENT_HPP

#include "Message.hpp"

class Event {
 public:
    using Action = std::function<void(const Message&, double)>;

    Event(double, const Message&,
        Action&&);
    void consume() const;
    bool operator<(const Event&) const; 

 private:
    double time;
    Message message;
    Action action;
};

inline Event::Event(double time, const Message& message, Action&& fn):
 time(time),
 message(message),
 action(std::move(fn)) { }

inline void Event::consume() const {
    action(message, time);
}

inline bool Event::operator<(const Event& rhs) const {
    return time < rhs.time;
}

#endif /* EVENT_HPP */
