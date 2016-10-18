

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

enum class Address {
    LOCAL,
    REMOTE,
};

enum class Status {
    SUCCESS,
    FAILURE,
    POSTPONED,
};

struct Message {
    using Type = Address;
    Status curr_status;
    Address from;
    Address to;
};

#endif /* MESSAGE_HPP */
