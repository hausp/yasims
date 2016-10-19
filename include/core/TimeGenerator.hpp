
#ifndef EVENT_FACTORY_HPP
#define EVENT_FACTORY_HPP

#include <functional>
#include <unordered_map>

#include "random/Function.hpp"

template<typename T>
class TimeGenerator {
    using AddrMap = std::unordered_map<T, dist::funct<>>;
 public:
    TimeGenerator(AddrMap);
    double generate(const T&) const;
 private:
    AddrMap times;
};

template<typename T>
inline TimeGenerator<T>::TimeGenerator(AddrMap times):
    times{std::move(times)} {}

template<typename T>
inline double TimeGenerator<T>::generate(const T& addr) const {
    return times.at(addr)();
}

#endif /* EVENT_FACTORY_HPP */
