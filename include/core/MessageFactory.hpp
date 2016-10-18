
#ifndef MESSAGE_FACTORY_HPP
#define MESSAGE_FACTORY_HPP

#include <functional>
#include "Message.hpp"

class MessageFactory {
 public:
    Message create() const;
 private:
    // TODO
};

#endif /* MESSAGE_FACTORY_HPP */
