
#ifndef EVENT_FACTORY_HPP
#define EVENT_FACTORY_HPP

#include <functional>

#include "Event.hpp"
#include "MessageFactory.hpp"
#include "random/Function.hpp"

class EventFactory {
 public:
    EventFactory(MessageFactory&, const dist::funct<>&);

    Event create(double, Event::Action&&) const;
 private:
    MessageFactory& message_factory;
    dist::funct<> arrival_time_fn;
};

#endif /* EVENT_FACTORY_HPP */
