
#ifndef SMAIL_PROCESSING_CENTER_HPP
#define SMAIL_PROCESSING_CENTER_HPP

#include <queue>
#include <set>
#include "Types.hpp"
#include "Message.hpp"

namespace smail {
    class ProcessingCenter {
     public:
        ProcessingCenter(MFMap, size_t = 1);

        Event receive(Message);
        std::pair<Message, bool> dispatch();
        void reset();
     private:
        size_t capacity;
        MsgQueue allocated;
        std::queue<Message> waiting_queue;
        MFMap processing_times;
    };
}

#endif /* SMAIL_PROCESSING_CENTER_HPP */
