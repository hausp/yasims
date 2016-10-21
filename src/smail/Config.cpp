
#include "smail/Config.hpp"

void smail::Config::synchronize_seed() {
    for (auto& fn : arrival_times) {
        // fn.gen = decltype(fn.gen){seed};
    }
}
