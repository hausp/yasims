
#ifndef DIST_TYPES_HPP
#define DIST_TYPES_HPP

#include <random>
#include <functional>

#include "Distribution.hpp"

namespace dist {
    struct Global {
        static std::random_device RD;
        static size_t SEED;
    };

    template<typename, typename = double>
    class Function;

    // Types of random distribution functions
    enum class Type {
        NORM,   // Normal distribution
        UNIF,   // Uniform distribution
        TRIA,   // Triangular distribution
        EXPO,   // Exponential distribution
        DISC,   // Discrete distribution
        CONS,   // Constant distribution
    };

    // Normal distribution alias
    template<typename T = double>
    using norm = Function<std::normal_distribution<T>, T>;
    // Uniform distribution alias
    template<typename T = double>
    using unif = Function<std::uniform_real_distribution<T>, T>;
    // Triangular distribution alias
    template<typename T = double>
    using tria = Function<dist::triangular_distribution<T>, T>;
    // Exponential distribution alias
    template<typename T = double>
    using expo = Function<std::exponential_distribution<T>, T>;
    // Discrete distribution alias
    template<typename T = double>
    using disc = Function<dist::discrete_distribution<T>, T>;
    // Constant distribution alias
    template<typename T = double>
    using cons = Function<dist::constant_distribution<T>, T>;

    // std::function encapsulation alias
    template<typename T = double>
    using funct = std::function<T(bool)>;

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

    /// Discrete distribution specialization
    template<typename T>
    struct ByType<Type::DISC, T> {
        using function = Function<dist::discrete_distribution<T>, T>;
    };

    /// Discrete distribution specialization
    template<typename T>
    struct ByType<Type::CONS, T> {
        using function = Function<dist::constant_distribution<T>, T>;
    };

    // Translates Type to function in running time
    template<typename T = double, typename... Args>
    funct<T> type_to_function(const Type& type, Args&&... args) {
        switch(type) {
            case Type::NORM:
                return norm<T>(std::forward<Args>(args)...);
            case Type::UNIF:
                return unif<T>(std::forward<Args>(args)...);
            case Type::TRIA:
                return tria<T>(std::forward<Args>(args)...);
            case Type::EXPO:
                return expo<T>(std::forward<Args>(args)...);
            case Type::DISC:
                return disc<T>(std::forward<Args>(args)...);
            case Type::CONS:
                return cons<T>(std::forward<Args>(args)...);
        }
    }

    template<typename T = double>
    funct<T> type_to_function(const Type& type, std::vector<double> values) {
        switch(type) {
            case Type::NORM:
                return norm<T>(values[0], values[1]);
            case Type::UNIF:
                return unif<T>(values[0], values[1]);
            case Type::TRIA:
                return tria<T>(values[0], values[1], values[2]);
            case Type::EXPO:
                return expo<T>(values[0]);
            case Type::CONS:
                return cons<T>(values[0]);
            default: return funct<T>();
        }
    }
}

namespace std {
    template<>
    struct hash<dist::Type> {
        inline size_t operator()(dist::Type type) const {
            return hash<int>()(static_cast<int>(type));
        }
    };
}

#endif /* DIST_TYPES_HPP */
