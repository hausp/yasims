
#include "smail/ProcessingCenter.hpp"
#include "smail/Event.hpp"

smail::ProcessingCenter::ProcessingCenter(MFMap times, size_t capacity):
 capacity{capacity},
 processing_times{times} { }

smail::Event smail::ProcessingCenter::receive(Message msg) {
    auto processing_time = processing_times.at(msg)();
    if (allocated.size() < capacity) {
        msg.current_time += processing_time;
        allocated.insert(std::move(msg));   
    } else {
        if (waiting_queue.empty()) {
            // Get nearest to end message
            auto a = *allocated.begin();
            msg.current_time = a.current_time + processing_time;
        } else {
            // Get the last of queue
            auto last = waiting_queue.back();
            msg.current_time = last.current_time + processing_time;
        }
        waiting_queue.push(std::move(msg));
    }
    return {msg.current_time};
}

std::pair<smail::Message, bool> smail::ProcessingCenter::dispatch() {
    if (allocated.empty()) {
        return {Message{}, false};
    }

    auto msg_it = allocated.begin();
    auto msg = *msg_it;
    allocated.erase(msg_it);

    if (!waiting_queue.empty()) {
        allocated.insert(std::move(waiting_queue.front()));
        waiting_queue.pop(); 
    }

    return {std::move(msg), true};
}
