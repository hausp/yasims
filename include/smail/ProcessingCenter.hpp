
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
        size_t occupation() const;
        size_t in_queue() const;
     private:
        size_t capacity;
        MsgQueue allocated;
        std::queue<Message> waiting_queue;
        MFMap processing_times;
    };

    inline size_t ProcessingCenter::occupation() const {
        return allocated.size();
    }

    inline size_t ProcessingCenter::in_queue() const {
        return waiting_queue.size();        
    }
}

#endif /* SMAIL_PROCESSING_CENTER_HPP */
