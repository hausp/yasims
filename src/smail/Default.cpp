
#include "smail/Default.hpp"

std::random_device smail::Default::RD;

const dist::disc<smail::Address> smail::Default::L_DESTINATION = {
    {Address::LOCAL, Address::REMOTE},
    {0.667, 0.333}
};
const dist::disc<smail::Address> smail::Default::R_DESTINATION = {
    {Address::LOCAL, Address::REMOTE},
    {0.60, 0.40}
};

const dist::funct<> smail::Default::L_ARRIVAL_TIMES = dist::expo<>{0.6};
const dist::funct<> smail::Default::R_ARRIVAL_TIMES = dist::expo<>{0.5};

const smail::AWMap smail::Default::L_STATUS_WEIGHTS = {
    {Address::LOCAL, {0.87, 0.005, 0.125}},
    {Address::REMOTE, {0.96, 0.015, 0.025}}
};

const smail::AWMap smail::Default::R_STATUS_WEIGHTS = {
    {Address::LOCAL, {0.96, 0.03, 0.01}},
    {Address::REMOTE, {0.90, 0.01, 0.09}}
};

const smail::MFMap smail::Default::RECEPTION_TIMES = {
    {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
     dist::cons<>{0.14}},
    {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
     dist::cons<>{0.11}},
    {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
     dist::cons<>{0.13}},
    {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
     dist::cons<>{0.15}},
    {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
     dist::cons<>{0.12}},
    {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
     dist::cons<>{0.14}},
    {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
     dist::cons<>{0.11}},
    {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
     dist::cons<>{0.16}},
    {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
     dist::cons<>{0.13}},
    {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
     dist::cons<>{0.16}}
};

const smail::MFMap smail::Default::L_PROCESSING_TIMES = {
    {{Address::LOCAL, Address::LOCAL, Status::SUCCESS},
     dist::norm<>{RD(), 0.55, 0.05}},
    {{Address::LOCAL, Address::LOCAL, Status::FAILURE},
     dist::tria<>{RD(), 0.02, 0.05, 0.12}},
    {{Address::LOCAL, Address::LOCAL, Status::POSTPONED},
     dist::unif<>{RD(), 0.06, 0.15}},
    {{Address::LOCAL, Address::REMOTE, Status::SUCCESS},
     dist::norm<>{RD(), 0.65, 0.04}},
    {{Address::LOCAL, Address::REMOTE, Status::FAILURE},
     dist::unif<>{RD(), 0.16, 0.25}},
    {{Address::LOCAL, Address::REMOTE, Status::POSTPONED},
     dist::tria<>{RD(), 0.05, 0.07, 0.10}}
};

const smail::MFMap smail::Default::R_PROCESSING_TIMES = {
    {{Address::REMOTE, Address::LOCAL, Status::SUCCESS},
     dist::unif<>{RD(), 0.03, 0.11}},
    {{Address::REMOTE, Address::LOCAL, Status::FAILURE},
     dist::norm<>{RD(), 0.46, 0.05}},
    {{Address::REMOTE, Address::LOCAL, Status::POSTPONED},
     dist::norm<>{RD(), 0.72, 0.09}},
    {{Address::REMOTE, Address::REMOTE, Status::SUCCESS},
     dist::unif<>{RD(), 0.09, 0.18}},
    {{Address::REMOTE, Address::REMOTE, Status::FAILURE},
     dist::tria<>{RD(), 0.08, 0.15, 0.22}},
    {{Address::REMOTE, Address::REMOTE, Status::POSTPONED},
     dist::norm<>{RD(), 0.63, 0.04}}
};