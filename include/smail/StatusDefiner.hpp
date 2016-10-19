
#ifndef SMAIL_STATUS_DEFINER_HPP
#define SMAIL_STATUS_DEFINER_HPP

#include "Default.hpp"

namespace smail {
    class StatusDefiner {
        using ADMap = std::unordered_map<Address, dist::disc<Status>>;
     public:
        StatusDefiner(AWMap);
        void set_weights(Address, Weights);
        Status generate(Address, unsigned seed);
     private:
        ADMap status_disc;
    };
}

inline smail::StatusDefiner::StatusDefiner(AWMap weights):
 status_disc {
    {Address::LOCAL, dist::disc<Status>(
        {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
        std::move(weights[Address::LOCAL]))},
    {Address::REMOTE, dist::disc<Status>(
        {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
        std::move(weights[Address::REMOTE]))}} { }

inline void smail::StatusDefiner::set_weights(Address addr, Weights ws) {
    status_disc[addr] = {
        {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
        std::move(ws)
    };
}

inline smail::Status smail::StatusDefiner::generate(Address addr, unsigned seed) {
    return status_disc.at(addr)(seed);
}

/*

*/

#endif /* SMAIL_STATUS_DEFINER_HPP */
