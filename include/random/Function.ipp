
template<typename Fn, typename T>
template<typename... Args>
dist::Function<Fn,T>::Function(int seed, Args&&... args):
 function{std::forward<Args>(args)...},
 seed{seed} { }

template<typename Fn, typename T>
template<typename... Args>
dist::Function<Fn,T>::Function(Args&&... args):
 function{std::forward<Args>(args)...} {
    std::random_device rd;
    seed = rd();
}

template<typename Fn, typename T>
T dist::Function<Fn,T>::operator()() {
    std::mt19937_64 gen{seed};
    return function(gen);
}

template<typename T>
dist::Function<dist::constant_distribution<T>, T>::Function(int, T value):
    function{value} { }

template<typename T>
dist::Function<dist::constant_distribution<T>, T>::Function(T value):
    function{value} { }

template<typename T>
constexpr T dist::Function<dist::constant_distribution<T>, T>::operator()() {
    return function.value;
}
