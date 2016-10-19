
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
        constexpr Function(int, Args&&...);
        template<typename... Args>
        constexpr Function(Args&&...);
        T operator()();

        Fn function;
        int seed;
    };

    template<typename T>
    struct Function<constant_distribution<T>, T> {
        constexpr Function(int, T); // For compatibility purposes
        constexpr Function(T);
        constexpr T operator()();

        constant_distribution<T> function;
    };
}

#include "Function.ipp"

#endif /* DIST_FUNCTION_HPP */
