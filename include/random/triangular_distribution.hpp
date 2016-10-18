
#ifndef DIST_TRIANGULAR_DISTRIBUTION_HPP
#define DIST_TRIANGULAR_DISTRIBUTION_HPP

namespace dist {
    /// Triangular function
    // Missing in stdlib, so...
    template<typename T = double>
    struct triangular_distribution {
        template<typename Generator>
        T operator()(Generator&) const;

        T lower_limit;
        T upper_limit;
        T mode;
    };
}

#include "triangular_distribution.ipp"

#endif /* DIST_TRIANGULAR_DISTRIBUTION_HPP */
