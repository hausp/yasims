
#include "smail/Default.hpp"

const smail::Default::AFMap smail::Default::arrival_times = {
    {Address::LOCAL,
     dist::expo<>{seed, 0.6}},
    {Address::REMOTE,
     dist::expo<>{seed, 0.5}}
};

const smail::Default::MFMap smail::Default::reception_times = {
    {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
     dist::cons<>{0.12}}
};

const smail::Default::MFMap smail::Default::processing_times = {
    {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
     dist::cons<>{0.12}}
};