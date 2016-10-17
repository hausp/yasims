#ifndef AW_SIGNAL_HPP
#define AW_SIGNAL_HPP

#include <utility>

namespace aw {
    template<typename Receiver,
             typename Receiver::Instance = typename Receiver::Instance()>
    class Signal {
        template<typename R, typename... As>
        using call = R(Receiver::*)(As...);
     public:
        static void set_receiver(Receiver&);

        template<typename>
        struct function;

        template<typename R, typename... As>
        struct function<R(As...)> {
            template<call<R, As...> Fn>
            static R callback(As... args) {
                return (receiver->*Fn)(args...);
            }
        };

     private:
        static Receiver* receiver;
    };
}

template<class Receiver, typename Receiver::Instance Instance>
Receiver* aw::Signal<Receiver, Instance>::receiver;

template<class Receiver, typename Receiver::Instance Instance>
void aw::Signal<Receiver, Instance>::set_receiver(Receiver& rec) {
    receiver = &rec;
}

#endif /* AW_SIGNAL_HPP */
