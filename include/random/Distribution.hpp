
#ifndef DIST_DISTRIBUTION_HPP
#define DIST_DISTRIBUTION_HPP

#include <random>

namespace dist {
    /// Constant function
    template<typename T>
    struct constant_distribution {
        T value;
    };

    /// Discrete function
    template<typename V>
    struct discrete_distribution {
        template<typename Generator>
        V operator()(Generator&);

        std::vector<V> values;
        std::discrete_distribution<int> dist_fn;
    };

    /// Triangular function
    // Missing in stdlib, so...
    template<typename T = double>
    struct triangular_distribution {
        template<typename Generator>
        T operator()(Generator&) const;

        T a; // lower_limit;
        T b; // mode;
        T c; // upper_limit;
    };
}

#include "Distribution.ipp"

#endif /* DIST_DISTRIBUTION_HPP */
