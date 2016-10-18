
#ifndef DIST_DISCRETE_DISTRIBUTION_HPP
#define DIST_DISCRETE_DISTRIBUTION_HPP

template<typename T>
struct discrete_distribution {
    template<typename Generator>
    T operator()(Generator&) const;

    int seed;
    std::unordered_map<T, double> distribution;
};

#endif /* DIST_DISCRETE_DISTRIBUTION_HPP */
