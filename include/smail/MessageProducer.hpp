
#ifndef SMAIL_MESSAGE_PRODUCER_HPP
#define SMAIL_MESSAGE_PRODUCER_HPP

#include "Default.hpp"
#include "Event.hpp"
#include "Message.hpp"

namespace smail {
    class MessageProducer {
        using ADMap = std::unordered_map<Address, dist::disc<Status>>;
     public:
        MessageProducer(Address, dist::funct<>, AWMap);
        void set_arrival_time(dist::funct<>);
        void set_status_disc(Address, Weights);
        Event produce();
     private:
        MsgQueue incoming_messages;
        Address from;
        dist::funct<> arrival_time;
        ADMap status_disc;
    };
}

inline smail::MessageProducer::MessageProducer(Address from,
                                               dist::funct<> fn,
                                               AWMap weights):
    from{from},
    arrival_time {std::move(fn)},
    status_disc {
        {Address::LOCAL,
        {std::vector<Status>{Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
         std::move(weights[Address::LOCAL])}},
        {Address::REMOTE,
        {std::vector<Status>{Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
         std::move(weights[Address::REMOTE])}}} { }

inline void smail::MessageProducer::set_arrival_time(dist::funct<> fn) {
    arrival_time = std::move(fn);
}

inline void smail::MessageProducer::set_status_disc(Address addr, Weights weights) {
    status_disc[addr] = {
        std::vector<Status>{Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
        std::move(weights)
    };
}

inline smail::Event smail::MessageProducer::produce() {
    auto message = Message {
        from
    };
    return Event();
}

#endif /* SMAIL_MESSAGE_PRODUCER_HPP */
