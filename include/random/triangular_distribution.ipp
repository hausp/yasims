
template<typename T>
template<typename Generator>
T dist::triangular_distribution<T>::operator()(Generator& gen) const {
    T r = gen();
    if (r > (mode - lower_limit)/(upper_limit - lower_limit) ) {
    	r = upper_limit - ((1 - r)*(upper_limit - mode)*(upper_limit - lower_limit))^(0.5);
    } else {
    	r = lower_limit + (r*(mode - lower_limit)*(upper_limit - lower_limit))^(0.5);
    }

    return r;
}