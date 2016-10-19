template<typename Fn, typename T>
template<typename... Args>
constexpr dist::Function<Fn,T>::Function(Args... args):
 function{std::forward<Args>(args)...} { }

template<typename Fn, typename T>
T dist::Function<Fn,T>::operator()(unsigned seed) {
    std::mt19937_64 gen{seed};
    return function(gen);
}

template<typename T>
constexpr dist::disc<T>::Function():
    function{dist::discrete_distribution<T>()} { }

template<typename T>
constexpr dist::disc<T>::Function(VList vs, WList ws):
    function{std::move(vs), std::move(ws)} { }

template<typename T>
T dist::disc<T>::operator()(unsigned seed) {
    std::mt19937_64 gen{seed};
    return function(gen);
}

template<typename T>
constexpr dist::cons<T>::Function(T value):
    function{value} { }

template<typename T>
constexpr T dist::cons<T>::operator()(unsigned) {
    return function.value;
}
