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

std::string smail::MessageConsumer::reveal_info() {
    std::string out = "";
    out += "O total de mensagens despachadas foi: ";
    out += std::to_string(total_exited);
    out += "\nA mensagem que passou menos tempo no sistema foi despachada em: ";
    out += std::to_string(faster_msg_time); 
    out += " segundos\n";
    out += "A mensagem que passou mais tempo no sistema foi despachada em: ";
    out += std::to_string(slower_msg_time);
    out += " segundos\nO tempo médio das mensagens no sistema foi de: ";
    out += std::to_string(avg_system_time);
    out += " segundos\n";  
    out += "Total de mensagens despachadas por tipo: \n";
    for (auto pair : exited_messages) {
        out += to_string(pair.first.from);
        out += "->";
        out += to_string(pair.first.to);
        out += ": ";
        out += to_string(pair.first.status);
        out += ": ";
        out += pair.second;
        out += "\n";
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