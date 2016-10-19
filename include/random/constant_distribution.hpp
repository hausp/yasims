
#ifndef DIST_CONSTANT_DISTRIBUTION_HPP
#define DIST_CONSTANT_DISTRIBUTION_HPP

#include <random>

namespace dist {
    template<typename T>
    struct constant_distribution {
        T value;
    };
}

#endif /* DIST_CONSTANT_DISTRIBUTION_HPP */
