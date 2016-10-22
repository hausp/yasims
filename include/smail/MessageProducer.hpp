
#ifndef SMAIL_MESSAGE_PRODUCER_HPP
#define SMAIL_MESSAGE_PRODUCER_HPP

#include "Types.hpp"
#include "Event.hpp"
#include "Message.hpp"

namespace smail {
    class MessageProducer {
     public:
        MessageProducer(Address, dist::funct<>, dist::disc<Address>);
        void set_arrival_time(dist::funct<>);
        Event produce(double = 0);
        Message dispatch();
        void reset();
     private:
        MsgQueue incoming_messages;
        Address from;
        dist::funct<> arrival_time;
        dist::disc<Address> destination;
    };
    
    inline MessageProducer::MessageProducer(Address from, dist::funct<> arrival,
                                            dist::disc<Address> dest):
     from{from},
     arrival_time{std::move(arrival)},
     destination{std::move(dest)} { }

    inline void MessageProducer::set_arrival_time(dist::funct<> fn) {
        arrival_time = std::move(fn);
    }

    inline Message MessageProducer::dispatch() {
        auto message = incoming_messages.top();
        incoming_messages.pop();
        return message;
    }

    inline Event MessageProducer::produce(double time) {
        auto inbound_time = time + arrival_time(false);
        incoming_messages.emplace(Message{
            from,
            destination(false),
            Status::UNDEFINED,
            inbound_time
        });
        return Event{inbound_time};
    }

    inline void MessageProducer::reset() {
        auto cleaner = MsgQueue{};
        incoming_messages.swap(cleaner);
        arrival_time(true);
        destination(true);
    }
}


#endif /* SMAIL_MESSAGE_PRODUCER_HPP */
