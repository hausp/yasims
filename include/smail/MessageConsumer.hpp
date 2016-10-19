
#ifndef SMAIL_MESSAGE_CONSUMER_HPP
#define SMAIL_MESSAGE_CONSUMER_HPP

#include "Message.hpp"

namespace smail {
    class MessageConsumer {
     public:
        void consume(const Message&) const;
     private:

    };
}

#endif /* SMAIL_MESSAGE_CONSUMER_HPP */
