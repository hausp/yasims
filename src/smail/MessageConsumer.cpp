#include "smail/MessageConsumer.hpp"
#include <iostream>

void smail::MessageConsumer::consume(const Message& msg) {
    exited_messages[msg]++;
    total_exited++;
    avg_system_time += (msg.in_system_time - avg_system_time) / total_exited;
    if (msg.in_system_time < faster_msg_time) {
        faster_msg_time = msg.in_system_time;
    }
    if (msg.in_system_time > slower_msg_time) {
        slower_msg_time = msg.in_system_time;
    }
}

void smail::MessageConsumer::reveal_info() const {
    std::cout << "--- End of Execution Report ---\n";
    std::cout << "The total amount of exited messages was: " 
              << total_exited << std::endl;
    std::cout << "The message that spent less time spent: "
              << faster_msg_time << "s in system\n";
    std::cout << "The message that spent the most time spent: "
              << slower_msg_time << "s in system\n";
    std::cout << "The average time spent by each message was: "
              << avg_system_time << "s in system\n"; 
    std::cout << "Total amount of messages by type: \n";
    for (auto pair : exited_messages) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void smail::MessageConsumer::reset() {
    avg_system_time = 0;
    faster_msg_time = std::numeric_limits<double>::max();
    slower_msg_time = 0;
    total_exited = 0;
    exited_messages.clear();
}