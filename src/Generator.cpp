

#include "Generator.hpp"

Event Generator::generate(double time,
    std::function<void(const Message&, double)>&& fn) const {
    
    auto m = Message{
        Status::SUCCESS,
        Address::LOCAL,
        Address::LOCAL
    };
    // Gerar todas as coisas da mensagem
    return {time + arrival_time_fn(), m, std::move(fn)};
}
