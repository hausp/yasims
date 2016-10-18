
#ifndef FACTORY_HPP
#define FACTORY_HPP

template<typename T>
class Factory {
 public:
    template<typename... Args>
    T create(Args&&...) const;
};

template<typename T>
template<typename... Args>
T Factory<T>::create(Args&&... args) const {
    return T(std::forward<Args>(args)...);
}

#endif /* FACTORY_HPP */
