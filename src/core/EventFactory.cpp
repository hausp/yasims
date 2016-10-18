
#include "core/EventFactory.hpp"

EventFactory::EventFactory(MessageFactory& mf, const dist::funct<>& fn):
 message_factory{mf},
 arrival_time_fn{fn} { }

Event EventFactory::create(double time, Event::Action&& action) const {
    // TODO: implementation
    auto message = message_factory.create();
    return {time + arrival_time_fn(), message, std::move(action)};
}