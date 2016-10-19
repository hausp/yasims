
#ifndef DIST_DISCRETE_DISTRIBUTION_HPP
#define DIST_DISCRETE_DISTRIBUTION_HPP

#include <random>

namespace dist {
    template<typename V>
    class discrete_distribution {
        using ValuesList = std::initializer_list<V>;
        using WeightsList = std::initializer_list<double>;
     public:
        discrete_distribution(ValuesList, WeightsList);

        template<typename Generator>
        V operator()(Generator&);

     private:
        std::vector<V> values;
        std::discrete_distribution<int> dist_fn;
    };
}

#include "discrete_distribution.ipp"

#endif /* DIST_DISCRETE_DISTRIBUTION_HPP */
