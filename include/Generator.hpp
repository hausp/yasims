
#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <functional>
#include "Event.hpp"

class Generator {
 public:
    Generator(std::function<double()>&&);

    Event generate(double time,
        std::function<void(const Message&, double)>&& fn) const;
 private:
    std::function<double()> arrival_time_fn;
};

inline Generator::Generator(std::function<double()>&& fn):
 arrival_time_fn(std::move(fn)) { }

#endif /* GENERATOR_HPP */
