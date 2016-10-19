
#ifndef SMAIL_DEFAULT_HPP
#define SMAIL_DEFAULT_HPP

#include <unordered_map>
#include "Message.hpp"
#include "random/Function.hpp"

namespace smail {
    class Default {
     private:
        using MFMap = std::unordered_map<Message, dist::funct<>>;
        using AFMap = std::unordered_map<Address, dist::funct<>>;
     public:
        static const AFMap arrival_times;
        static const MFMap reception_times;
        static const MFMap processing_times;
        static constexpr int seed = 0;
    };
}

#endif /* SMAIL_DEFAULT_HPP */
