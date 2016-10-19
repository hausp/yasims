
template<typename V>
dist::discrete_distribution<V>::discrete_distribution(ValuesList values,
                                                      WeightsList weights):
    values{std::move(values)},
    dist_fn{std::move(weights)} {

}

template<typename V>
template<typename Generator>
V dist::discrete_distribution<V>::operator()(Generator& gen) {
    auto index = dist_fn(gen);
    return values.at(index);
}