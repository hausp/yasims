
#ifndef DIST_FUNCTION_HPP
#define DIST_FUNCTION_HPP

#include "Types.hpp"

namespace dist {
    template<typename Fn, typename T>
    struct Function {
        template<typename... Args>
        constexpr Function(Args...);
        template<typename... Args>
        constexpr Function(unsigned, Args...);
        
        T operator()(bool);

        Fn function;
        std::mt19937_64 gen;
    };

    template<typename T>
    struct Function<discrete_distribution<T>, T> {
        using VList = std::initializer_list<T>;
        using WList = std::initializer_list<double>;

        constexpr explicit Function();
        constexpr Function(VList, WList);
        constexpr Function(unsigned, VList, WList);

        T operator()(bool);

        discrete_distribution<T> function;
        std::mt19937_64 gen;
    };

    template<typename T>
    struct Function<constant_distribution<T>, T> {
        constexpr Function(T);
        constexpr T operator()(bool);

        constant_distribution<T> function;
    };
}

#include "Function.ipp"

#endif /* DIST_FUNCTION_HPP */
