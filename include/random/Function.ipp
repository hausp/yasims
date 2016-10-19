
template<typename Fn, typename T>
template<typename... Args>
constexpr dist::Function<Fn,T>::Function(Args... args):
 function{std::forward<Args>(args)...} { }

template<typename Fn, typename T>
template<typename... Args>
constexpr dist::Function<Fn,T>::Function(unsigned seed, Args... args):
 function{std::forward<Args>(args)...},
 gen{seed} { }

template<typename Fn, typename T>
T dist::Function<Fn,T>::operator()() {
    return function(gen);
}

template<typename T>
constexpr dist::disc<T>::Function():
 function{dist::discrete_distribution<T>()} { }

template<typename T>
constexpr dist::disc<T>::Function(VList vs, WList ws):
    function{std::move(vs), std::move(ws)} { }

template<typename T>
constexpr dist::disc<T>::Function(unsigned seed, VList vs, WList ws):
    function{std::move(vs), std::move(ws)},
    gen{seed} { }

template<typename T>
T dist::disc<T>::operator()() {
    return function(gen);
}

template<typename T>
constexpr dist::cons<T>::Function(T value):
    function{value} { }

template<typename T>
constexpr T dist::cons<T>::operator()() {
    return function.value;
}
