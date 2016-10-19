#include <cmath>

template<typename T>
template<typename Generator>
T dist::triangular_distribution<T>::operator()(Generator& gen) const {
    T r = gen();
    if (r > (b - a)/(c - a) ) {
    	r = c - std::sqrt((1 - r)*(c - b)*(c - a));
    } else {
    	r = a + std::sqrt(r*(b - a)*(c - a));
    }
    return r;
}