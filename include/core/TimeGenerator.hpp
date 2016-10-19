
#ifndef TIME_GENERATOR_HPP
#define TIME_GENERATOR_HPP

#include <functional>
#include <unordered_map>

#include "random/Function.hpp"

template<typename T>
class TimeGenerator {
    using Map = std::unordered_map<T, dist::funct<>>;
 public:
    TimeGenerator(Map);
    double generate(const T&) const;
 private:
    Map times;
};

template<typename T>
inline TimeGenerator<T>::TimeGenerator(Map times):
    times{std::move(times)} {}

template<typename T>
inline double TimeGenerator<T>::generate(const T& addr) const {
    return times.at(addr)();
}

#endif /* TIME_GENERATOR_HPP */
