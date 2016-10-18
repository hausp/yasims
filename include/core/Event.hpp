// 

#ifndef EVENT_HPP
#define EVENT_HPP

#include "Message.hpp"

class Event {
 public:
    Event(double, const Message&,
        std::function<void(const Message&, double)>&&);
    void consume() const;
    bool operator<(const Event&) const; 

 private:
    double time;
    Message message;
    std::function<void(const Message&, double)> action;
};

inline Event::Event(double time,
                    const Message& message,
                    std::function<void(const Message&, double)>&& fn):
time(time), message(message), action(std::move(fn)) { }

inline void Event::consume() const {
    action(message, time);
}

inline bool Event::operator<(const Event& rhs) const {
    return time < rhs.time;
}

#endif /* EVENT_HPP */
