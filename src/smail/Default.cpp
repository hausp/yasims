
#include "smail/Default.hpp"

std::random_device smail::Default::rd;

const dist::funct<>
smail::Default::local_arrival_time = dist::expo<>{Default::rd(), 0.6};

const dist::funct<>
smail::Default::remote_arrival_time = dist::expo<>{Default::rd(), 0.5};


const smail::AWMap smail::Default::local_status_weights = {
    {Address::LOCAL, {1.0, 1.0, 1.0}},
    {Address::REMOTE, {1.0, 1.0, 1.0}}
};

const smail::AWMap smail::Default::remote_status_weights = {
    {Address::LOCAL, {1.0, 1.0, 1.0}},
    {Address::REMOTE, {1.0, 1.0, 1.0}}
};

const smail::MFMap smail::Default::local_reception_times = {
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
     dist::cons<>{0.12}}
};

const smail::MFMap smail::Default::remote_reception_times = {
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

const smail::MFMap smail::Default::local_processing_times = {
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
     dist::cons<>{0.12}}
};
const smail::MFMap smail::Default::remote_processing_times = {
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