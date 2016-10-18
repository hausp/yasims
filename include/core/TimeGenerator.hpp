
#ifndef EVENT_FACTORY_HPP
#define EVENT_FACTORY_HPP

#include <functional>
#include <unordered_map>

#include "random/Function.hpp"

class TimeGenerator {
    using AddrMap = std::unordered_map<Address, dist::funct<>>;
 public:
    TimeGenerator(AddrMap);
    double generate(const Address&) const;
 private:
    AddrMap times;
};

inline TimeGenerator::TimeGenerator(AddrMap times):
    times{std::move(times)} {}

inline double TimeGenerator::generate(const Address& addr) const {
    return times.at(addr)();
}

#endif /* EVENT_FACTORY_HPP */
