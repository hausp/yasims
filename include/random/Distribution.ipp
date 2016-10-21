
#include <cmath>

template<typename V>
template<typename Generator>
V dist::discrete_distribution<V>::operator()(Generator& gen) {
    auto index = dist_fn(gen);
    return values.at(index);
}

template<typename T>
template<typename Generator>
T dist::triangular_distribution<T>::operator()(Generator& gen) const {
    T r = gen()/gen.max();
    if (r > (b - a)/(c - a) ) {
        r = c - std::sqrt((1 - r)*(c - b)*(c - a));
    } else {
        r = a + std::sqrt(r*(b - a)*(c - a));
    }
    return r;
}