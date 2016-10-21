
template<typename Fn, typename T>
template<typename... Args>
constexpr dist::Function<Fn,T>::Function(Args... args):
 function{std::forward<Args>(args)...}, gen{Global::SEED} { }


template<typename Fn, typename T>
T dist::Function<Fn,T>::operator()(bool update) {
    if (update) {
        gen = std::mt19937_64{Global::SEED};
        return T();
    }
    return function(gen);
}

template<typename T>
constexpr dist::disc<T>::Function():
 function{dist::discrete_distribution<T>()} { }

template<typename T>
constexpr dist::disc<T>::Function(VList vs, WList ws):
    function{std::move(vs), std::move(ws)}, gen{Global::SEED} { }

template<typename T>
T dist::disc<T>::operator()(bool update) {
    if (update) {
        gen = std::mt19937_64{Global::SEED};
        return T();
    }
    return function(gen);
}

template<typename T>
constexpr dist::cons<T>::Function(T value):
    function{value} { }

template<typename T>
constexpr T dist::cons<T>::operator()(bool) {
    return function.value;
}
