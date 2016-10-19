
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
     private:
        size_t capacity;
        std::set<Message> allocated;
        std::queue<Message> waiting_queue;
        MFMap processing_times;
    };
}

#endif /* SMAIL_PROCESSING_CENTER_HPP */
