
#ifndef DIST_TRIANGULAR_DISTRIBUTION_HPP
#define DIST_TRIANGULAR_DISTRIBUTION_HPP

namespace dist {
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

#include "triangular_distribution.ipp"

#endif /* DIST_TRIANGULAR_DISTRIBUTION_HPP */
