
#ifndef SMAIL_PROCESSING_CENTER_HPP
#define SMAIL_PROCESSING_CENTER_HPP

#include "Types.hpp"

namespace smail {
    class ProcessingCenter {
     public:
        ProcessingCenter(size_t);

        void receive(Message);
        Message dispatch(double);
     private:
        size_t capacity;
        MsgQueue allocated;
        MsgQueue waiting_queue;
    };
}

#endif /* SMAIL_PROCESSING_CENTER_HPP */
