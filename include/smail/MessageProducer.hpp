
#ifndef SMAIL_MESSAGE_PRODUCER_HPP
#define SMAIL_MESSAGE_PRODUCER_HPP

#include "Default.hpp"
#include "Event.hpp"
#include "Message.hpp"

namespace smail {
    class MessageProducer {
     public:
        MessageProducer(Address, dist::funct<>, dist::disc<Address>);
        void set_arrival_time(dist::funct<>);
        Event produce(double = 0, unsigned = 0);
     private:
        MsgQueue incoming_messages;
        Address from;
        dist::funct<> arrival_time;
        dist::disc<Address> destination;
    };
}

inline smail::MessageProducer::MessageProducer(Address from,
                                               dist::funct<> arrival,
                                               dist::disc<Address> dest):
    from{from},
    arrival_time {std::move(arrival)},
    destination{std::move(dest)} { }

inline void smail::MessageProducer::set_arrival_time(dist::funct<> fn) {
    arrival_time = std::move(fn);
}

inline smail::Event smail::MessageProducer::produce(double time, unsigned seed) {
    auto inbound_time = time + arrival_time(seed);
    incoming_messages.emplace(Message{
        from,
        destination(seed),
        Status::UNDEFINED,
        inbound_time
    });
    return Event{inbound_time};
}

#endif /* SMAIL_MESSAGE_PRODUCER_HPP */
