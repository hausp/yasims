
#ifndef MESSAGE_FACTORY_HPP
#define MESSAGE_FACTORY_HPP

#include <functional>
#include "Factory.hpp"
#include "Message.hpp"

template<>
class Factory<Message> {
 public:
    Message create(const Address&) const;
 private:
    // TODO
};

#endif /* MESSAGE_FACTORY_HPP */
