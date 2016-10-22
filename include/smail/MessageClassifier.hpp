
#ifndef SMAIL_MESSAGE_CLASSIFIER_HPP
#define SMAIL_MESSAGE_CLASSIFIER_HPP

#include "Types.hpp"

namespace smail {
    class MessageClassifier {
        using DistArray = std::array<dist::disc<Status>, 4>;
     public:
        MessageClassifier(AWMap, AWMap);

        void classify(Message&);
        void reset();
     private:
        DistArray status_disc;

        static constexpr size_t to_index(Address f, Address t) {
            auto i = static_cast<size_t>(f);
            auto j = static_cast<size_t>(t);
            return 2 * i + j;
        }
    };

    inline MessageClassifier::MessageClassifier(AWMap lweights, AWMap rweights):
     status_disc {{
        dist::disc<Status>{
            {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
            std::move(lweights[Address::LOCAL])
        },
        dist::disc<Status>{
            {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
            std::move(lweights[Address::REMOTE])
        },
        dist::disc<Status>{
            {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
            std::move(rweights[Address::LOCAL])
        },
        dist::disc<Status>{
            {Status::SUCCESS, Status::FAILURE, Status::POSTPONED},
            std::move(rweights[Address::REMOTE])
        }
    }} { }

    inline void MessageClassifier::classify(Message& msg) { 
        msg.status = status_disc[to_index(msg.from, msg.to)](false);
    }

    inline void MessageClassifier::reset() {
        for (auto& fn : status_disc) {
            fn(true);
        }
    }
}

#endif /* SMAIL_MESSAGE_CLASSIFIER_HPP */
