
#ifndef SMAIL_PROCESSING_CENTER_HPP
#define SMAIL_PROCESSING_CENTER_HPP

#include "Types.hpp"

namespace smail {
    struct resource {

    };

    class ProcessingCenter {
     public:

     private:
        MsgQueue allocated;
        MsgQueue waiting_queue;
    };
}

#endif /* SMAIL_PROCESSING_CENTER_HPP */
