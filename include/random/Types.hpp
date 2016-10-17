
#ifndef DIST_TYPES_HPP
#define DIST_TYPES_HPP

#include <random>
#include "triangular_distribution.hpp"

namespace dist {
    template<typename, typename = double>
    struct Function;

    // Types of random distribution functions
    enum class Type {
        NORM,   // Normal distribution
        UNIF,   // Uniform distribution
        TRIA,   // Triangular distribution
        EXPO,   // Exponential distribution
    };

    // Normal distribution alias
    using norm = Function<std::normal_distribution<>>;
    // Uniform distribution alias
    using unif = Function<std::uniform_real_distribution<>>;
    // Triangular distribution alias
    using tria = Function<dist::triangular_distribution<>>;
    // Exponential distribution alias
    using expo = Function<std::exponential_distribution<>>;

    /// Struct ByType
    // Translates dist::Type specifications to its std equivalents
    // in compilation time
    template<Type, typename T = double>
    struct ByType;

    /// Normal distribution specialization
    template<typename T>
    struct ByType<Type::NORM, T> {
        using function = Function<std::normal_distribution<T>, T>;
    };

    /// Uniform distribution specialization
    template<typename T>
    struct ByType<Type::UNIF, T> {
        using function = Function<std::uniform_real_distribution<T>, T>;
    };

    /// Triangular distribution specialization
    template<typename T>
    struct ByType<Type::TRIA, T> {
        using function = Function<dist::triangular_distribution<T>, T>;
    };

    /// Exponential distribution specialization
    template<typename T>
    struct ByType<Type::EXPO, T> {
        using function = Function<std::exponential_distribution<T>, T>;
    };

    // Translates Type to function in running time
    template<typename T = double, typename... Args>
    std::function<T()> type_to_function(const Type& type, Args&&... args) {
        switch(type) {
            case Type::NORM:
                return norm(std::forward<Args>(args)...);
            case Type::UNIF:
                return unif(std::forward<Args>(args)...);
            case Type::TRIA:
                return tria(std::forward<Args>(args)...);
            case Type::EXPO:
                return expo(std::forward<Args>(args)...);
        }
    }
}

#endif /* DIST_TYPES_HPP */
