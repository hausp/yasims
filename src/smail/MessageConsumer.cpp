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

std::stringstream smail::MessageConsumer::reveal_info() {
    std::stringstream out;
    out << "O total de mensagens despachadas foi: " 
              << total_exited << std::endl;
    out << "A mensagem que passou menos tempo no sistema foi "
        << "despachada em: " << faster_msg_time << " segundos" << std::endl;
    out << "A mensagem que passou mais tempo no sistema foi"
        << "despachada em: " << slower_msg_time << " segundos" << std::endl;
    out << "O tempo médio das mensagens no sistema foi de: "
              << avg_system_time << " segundos" << std::endl; 
    out << "Total de mensagens despachadas por tipo: " << std::endl;
    for (auto pair : exited_messages) {
        out << pair.first << ": " << pair.second << std::endl;
    }
    return out;
}

void smail::MessageConsumer::reset() {
    avg_system_time = 0;
    faster_msg_time = std::numeric_limits<double>::max();
    slower_msg_time = 0;
    total_exited = 0;
    exited_messages.clear();
}