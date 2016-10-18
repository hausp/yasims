
#ifndef DIST_DISTRIBUTION_HPP
#define DIST_DISTRIBUTION_HPP

#include "Types.hpp"

namespace dist {
    /// Struct Function
    // Struct to store all information needed to call
    // the desired distribution
    template<typename Fn, typename T>
    struct Function {
     public:
        template<typename... Args>
        Function(int, Args&&...);
        template<typename... Args>
        Function(Args&&...);
        T operator()();
     private:
        Fn function;
        int seed;
    };
}

#include "Function.ipp"

#endif /* DIST_DISTRIBUTION_HPP */
