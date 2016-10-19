
#ifndef SMAIL_EVENT_CONSUMER_HPP
#define SMAIL_EVENT_CONSUMER_HPP

#include "Event.hpp"

namespace smail {
    class EventConsumer {
     public:
        void consume(const Event&) const;
     private:

    };
}

#endif /* SMAIL_EVENT_CONSUMER_HPP */
