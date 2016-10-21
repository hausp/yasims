
#ifndef SMAIL_MESSAGE_CONSUMER_HPP
#define SMAIL_MESSAGE_CONSUMER_HPP

#include "Message.hpp"
#include <limits>
#include <map>
#include <sstream>

namespace smail {
    class MessageConsumer {
     public:
        void consume(const Message&);
        std::stringstream reveal_info();
        void reset();
     private:
        double avg_system_time = 0;
        double faster_msg_time = std::numeric_limits<double>::max();
        double slower_msg_time = 0;
        size_t total_exited = 0;
        std::unordered_map<Message, size_t> exited_messages;
    };
}

#endif /* SMAIL_MESSAGE_CONSUMER_HPP */
