
#ifndef DIST_FUNCTION_HPP
#define DIST_FUNCTION_HPP

#include "Types.hpp"

namespace dist {
    template<typename Fn, typename T>
    struct Function {
        template<typename... Args>
        constexpr Function(Args...);
        T operator()(unsigned);

        Fn function;
    };

    template<typename T>
    struct Function<discrete_distribution<T>, T> {
        using VList = std::initializer_list<T>;
        using WList = std::initializer_list<double>;

        constexpr explicit Function();
        constexpr Function(VList, WList);
        T operator()(unsigned);

        discrete_distribution<T> function;
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
