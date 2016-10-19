
#ifndef DIST_DISTRIBUTION_HPP
#define DIST_DISTRIBUTION_HPP

#include "Types.hpp"

namespace dist {
    /// Struct Function
    // Struct to store all information needed to call
    // the desired distribution
    template<typename Fn, typename T>
    class Function {
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

    template<typename T>
    class Function<constant_distribution<T>, T> {
     public:
        Function(int, T); // For compatibility purposes
        Function(T);
        constexpr T operator()();
     private:
        constant_distribution<T> function;
    };
}

#include "Function.ipp"

#endif /* DIST_DISTRIBUTION_HPP */
