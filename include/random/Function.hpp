
#ifndef DIST_FUNCTION_HPP
#define DIST_FUNCTION_HPP

#include "Types.hpp"

namespace dist {
    /// Struct Function
    // Struct to store all information needed to call
    // the desired distribution
    template<typename Fn, typename T>
    struct Function {
        template<typename... Args>
        constexpr Function(unsigned, Args...);
        template<typename... Args>
        constexpr Function(Args...);
        T operator()(unsigned);
        T operator()();

        Fn function;
        unsigned seed;
    };

    template<typename T>
    struct Function<constant_distribution<T>, T> {
        constexpr Function(T);
        constexpr T operator()(unsigned = 0);

        constant_distribution<T> function;
    };
}

#include "Function.ipp"

#endif /* DIST_FUNCTION_HPP */
