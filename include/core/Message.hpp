

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
    Status curr_status;
    Address from;
    Address to;
};

#endif /* MESSAGE_HPP */
