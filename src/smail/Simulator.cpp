
#include <cassert>
#include "interface/GTKInterface.hpp"
#include "smail/Simulator.hpp"
#include "random/Function.hpp"
#include "wrapper/Signal.hpp"
#include <iostream>

smail::Simulator::Simulator():
 spawners{
    MessageProducer{Address::LOCAL, config.arrival_times[0],
        config.destinations[0]},
    MessageProducer{Address::REMOTE, config.arrival_times[0],
        config.destinations[1]}
 },
 classifier{config.status_weights[0], config.status_weights[1]},
 reception{config.reception_times},
 centers {
    ProcessingCenter{
        config.processing_times[0],
        config.center_capacities[0]
    },
    ProcessingCenter{
        config.processing_times[1],
        config.center_capacities[1]
    },
 },
 thread{&smail::Simulator::run, this} {
    aw::Signal<Simulator>::set_receiver(*this);
 }

smail::Simulator::~Simulator() {
    survive = false;
    cv.notify_all();

    if (thread.joinable()) {
        thread.join();
    }
}

void smail::Simulator::start(bool animate) {
    if (execute) return;
    if (stopped) reset();

    setup();

    execute = true;

    if (animate) {
        launch_gtk_function();
    } else {
        std::lock_guard<std::mutex> guard{mutex};        
        cv.notify_all();
    }
}

void smail::Simulator::step() {
    if (execute) return;
    if (stopped) reset();

    setup();

    simulate();
}

void smail::Simulator::pause() {
    execute = false;
}

void smail::Simulator::stop() {
    if (!execute) return;
    // Sinalize to stop executing
    execute = false;
    stopped = true;
    // consumer.reveal_info();
    // reveal_messages_info();
    // reveal_input_info();
    // avg_occupation(0); avg_occupation(1);
}

void smail::Simulator::run() {
    while (survive) {
        std::unique_lock<std::mutex> permission{mutex};
        // Synchronizing operations, to keep thread suspended
        // when paused and stoped
        cv.wait(permission, [this] { return !survive || execute; });
        permission.unlock();
        // execution loop
        while (execute && survive) {
            simulate();
        }
    }
}

// For animations, better be in gtk's thread
bool smail::Simulator::gtk_run() {
    simulate();
    // TODO: update screen
    show_statistics();
    return execute;
}

void smail::Simulator::simulate() { 
    // Get next event
    auto e = events.top();
    // Update simulation clock and statistics
    update(e.time); // NOTICE: if e.action() fails this will pass equal times
    // Execute event action and check if successful
    if (e.action()) {
        // Successful events are removed
        events.pop();
    }
    // Update execute control variable
    stopped = events.empty();
    execute = execute && !stopped
        && (config.infinite_simulation || clock >= config.sim_time);
}

void smail::Simulator::update(double new_time) {
    auto occupation = reception.occupation() + reception.in_queue();
    reception_occupation += (new_time - clock)*
            (reception.occupation() - reception_occupation)/(new_time);
    reception_queue_occu += (new_time - clock)*
            (reception.in_queue() - reception_queue_occu)/(new_time);        
    for (size_t i = 0; i < centers.size(); ++i) {
        occupation += centers[i].occupation() + centers[i].in_queue();
        occupation_on_centers[i] += (new_time - clock)*
            (centers[i].occupation() - occupation_on_centers[i])/(new_time);
        occupation_on_queues[i] += (new_time - clock)*
            (centers[i].in_queue() - occupation_on_queues[i])/(new_time);
    }

    system_occupation += (new_time - clock)*
        (occupation - system_occupation)/(new_time);
    update_messages();
    clock = new_time;
}

void smail::Simulator::setup() {
    // If there is no event
    if (events.empty()) {
        // Give the initial stimulus, one local arrive and one remote
        arrival_event(static_cast<size_t>(Address::LOCAL));
        arrival_event(static_cast<size_t>(Address::REMOTE));
    }
}

void smail::Simulator::reset() {
    if (config.use_random_seed) {
        dist::Global::SEED = dist::Global::RD();
    }
    // A brand new EventQueue to replace older one
    auto cleaner = EventQueue{};
    events.swap(cleaner);

    classifier.reset();
    reception.reset();
    consumer.reset();
    msgs_in_system = 0;
    max_msgs_in_system = 0;
    min_msgs_in_system = 10;

    for (auto& spawner : spawners) {
        spawner.reset();
    }

    for (auto& center : centers) {
        center.reset();
    }
}

void smail::Simulator::arrival_event(size_t index) {
    // Produce message and its arrival event
    auto event = spawners[index].produce(clock);
    // Set arrival event action
    event.action = [this, index] {
        // Creates next arrival event
        arrival_event(index);
        // Dispatch a message from the spawner
        auto msg = spawners[index].dispatch();
        // Classify this message (choose its status)
        classifier.classify(msg);
        // Send message to reception
        reception_event(std::move(msg));
        // TODO
        return true;
    };
    events.push(std::move(event));
}

void smail::Simulator::reception_event(Message msg) {
    // Increment #msgs in system
    msgs_in_system++;
    // Update info that is received by the system
    update_input_info(msg);
    // Send message to reception and get ready event
    auto event = reception.receive(std::move(msg));
    // Set ready event action
    event.action = [this] {
        // Get dispatched message
        Message msg;
        bool dispatched;
        std::tie(msg, dispatched) = reception.dispatch();
        // temporary assert
        assert(dispatched);
        // Send message to processing centers
        processing_event(std::move(msg));
        // TODO
        return true;
    };
    events.push(std::move(event));
}

void smail::Simulator::processing_event(Message msg) {
    // Get index for the corresponding center
    auto index = static_cast<size_t>(msg.from);
    // Send message to center and get ready event
    auto event = centers[index].receive(std::move(msg));
    // Set ready event action
    event.action = [this, index] {
        // Get dispatched message
        Message msg;
        bool dispatched;
        std::tie(msg, dispatched) = centers[index].dispatch();
        // temporary assert
        assert(dispatched);
        // Check if has been postponed
        if (msg.status != Status::POSTPONED) {
            // If not, send message to exit
            exit_event(std::move(msg));
        } else {
            // Otherwise, send to postpone treatment
            postpone_event(std::move(msg));
        }
        // TODO
        return true;
    };
    events.push(std::move(event));
}

void smail::Simulator::postpone_event(Message msg) {
    if (msg.in_system_time >= config.timeout) {
        // Timeout = FAILURE
        msg.status = Status::FAILURE;
        // Send message to exit
        exit_event(std::move(msg));
    } else {
        // Reclassify the message
        classifier.classify(msg);
        // Resend to processing centers
        processing_event(std::move(msg));
    }
}

void smail::Simulator::exit_event(Message msg) {
    std::cout << "Exit event" << std::endl;
    // Decrement #msgs in system
    msgs_in_system--;
    // Process dispatched msg data
    consumer.consume(msg);
}

void smail::Simulator::update_messages() {
    if (msgs_in_system > max_msgs_in_system) {
        max_msgs_in_system = msgs_in_system;
    }
    if (msgs_in_system < min_msgs_in_system) {
        min_msgs_in_system = msgs_in_system;
    }
}

void smail::Simulator::update_input_info(const Message& msg) {
    if (!input_info.count(msg)) {
        input_info[msg] = 0;
    }
    input_info[msg]++;
}

void smail::Simulator::launch_gtk_function() {
    gtk_handler = g_timeout_add_full(
        G_PRIORITY_HIGH,
        speed,
        (GSourceFunc)(aw::Signal<Simulator>::function<bool()>
            ::callback<&Simulator::gtk_run>),
        nullptr,
        nullptr
    );
}

void smail::Simulator::update_speed(double d) {
    speed = (Default::SPEED_NUMERATOR + 1) / (d + 1);
    if (execute) {
        g_source_remove(gtk_handler);
        launch_gtk_function();
    }
}

void smail::Simulator::update_config(Config c) {
    reset();
    config = std::move(c);
     spawners = {
        MessageProducer{
            Address::LOCAL, config.arrival_times[0],
            config.destinations[0]
        },
        MessageProducer{
            Address::REMOTE, config.arrival_times[0],
            config.destinations[1]
        }
     };
     classifier = {
        config.status_weights[0],
        config.status_weights[1]
    };
    
    reception = {config.reception_times};
    
    centers = {
        ProcessingCenter{
            config.processing_times[0],
            config.center_capacities[0]
        },
        ProcessingCenter{
            config.processing_times[1],
            config.center_capacities[1]
        },
    };
}

void smail::Simulator::show_statistics() {
    auto statistics = std::array<std::string, 43> {
        std::to_string(clock),
        std::to_string(msgs_in_system),
        std::to_string(min_msgs_in_system),
        std::to_string(max_msgs_in_system),
        std::to_string(system_occupation),
        std::to_string(get_input_info(Address::LOCAL, Address::LOCAL)),
        std::to_string(get_input_info(Address::LOCAL, Address::REMOTE)),
        std::to_string(get_input_info(Address::REMOTE, Address::LOCAL)),
        std::to_string(get_input_info(Address::REMOTE, Address::REMOTE)),
        std::to_string(input_info[(Message{Address::LOCAL, Address::LOCAL, Status::SUCCESS})]),
        std::to_string(input_info[(Message{Address::LOCAL, Address::LOCAL, Status::FAILURE})]),
        std::to_string(input_info[(Message{Address::LOCAL, Address::LOCAL, Status::POSTPONED})]),
        std::to_string(input_info[(Message{Address::LOCAL, Address::REMOTE, Status::SUCCESS})]),
        std::to_string(input_info[(Message{Address::LOCAL, Address::REMOTE, Status::FAILURE})]),
        std::to_string(input_info[(Message{Address::LOCAL, Address::REMOTE, Status::POSTPONED})]),
        std::to_string(input_info[(Message{Address::REMOTE, Address::LOCAL, Status::SUCCESS})]),
        std::to_string(input_info[(Message{Address::REMOTE, Address::LOCAL, Status::FAILURE})]),
        std::to_string(input_info[(Message{Address::REMOTE, Address::LOCAL, Status::POSTPONED})]),
        std::to_string(input_info[(Message{Address::REMOTE, Address::REMOTE, Status::SUCCESS})]),
        std::to_string(input_info[(Message{Address::REMOTE, Address::REMOTE, Status::FAILURE})]),
        std::to_string(input_info[(Message{Address::REMOTE, Address::REMOTE, Status::POSTPONED})]),
        std::to_string(reception.in_queue()),
        std::to_string(reception_queue_occu),
        std::to_string(centers[0].in_queue()),
        std::to_string(occupation_on_queues[0]),
        std::to_string(centers[0].occupation()),
        std::to_string(occupation_on_centers[0]),
        std::to_string(centers[1].in_queue()),
        std::to_string(occupation_on_queues[1]),
        std::to_string(centers[1].occupation()),
        std::to_string(occupation_on_centers[1]),
        std::to_string(consumer.see_exited(Message{Address::LOCAL, Address::LOCAL, Status::SUCCESS})),
        std::to_string(consumer.see_exited(Message{Address::LOCAL, Address::LOCAL, Status::FAILURE})),
        std::to_string(consumer.see_exited(Message{Address::LOCAL, Address::REMOTE, Status::SUCCESS})),
        std::to_string(consumer.see_exited(Message{Address::LOCAL, Address::REMOTE, Status::FAILURE})),
        std::to_string(consumer.see_exited(Message{Address::REMOTE, Address::LOCAL, Status::SUCCESS})),
        std::to_string(consumer.see_exited(Message{Address::REMOTE, Address::LOCAL, Status::FAILURE})),
        std::to_string(consumer.see_exited(Message{Address::REMOTE, Address::REMOTE, Status::SUCCESS})),
        std::to_string(consumer.see_exited(Message{Address::REMOTE, Address::REMOTE, Status::FAILURE})),
        std::to_string(consumer.see_total()),
        //transito
        std::to_string(consumer.get_faster()),
        std::to_string(consumer.get_slower()),
        std::to_string(consumer.get_avg_sys_time())
    };
    aw::Signal<GTKInterface>::function<void(std::array<std::string, 43>)>
        ::callback<&GTKInterface::show_statistics>(std::move(statistics));
}

// std::stringstream smail::Simulator::reveal_messages_info() {
//     std::stringstream out;
//     out << "Mensagens restantes nos sistema: " << msgs_in_system 
//         << " mensagens" << std::endl;;
//     out << "Máximo de mensagens no sistema: "
//         << max_msgs_in_system << " mensagens" << std::endl;;
//     out << "Mínimo de mensagens no sistema: "
//         << min_msgs_in_system << " mensagens" << std::endl;;
//     out << "Número médio de mensagens no sistema: "
//         << system_occupation << "" << std::endl;;
//     return out;
// }

// std::stringstream smail::Simulator::avg_occupation(size_t index) {
//     std::stringstream out;
//     out << "A ocupação média no centro " << index+1 <<
//         " foi: " << occupation_on_centers[index] << "" << std::endl;
//     out << "E o tamanho médio da fila foi: " 
//         << occupation_on_queues[index] << "" << std::endl;
//     return out;
// }

// std::stringstream smail::Simulator::reveal_input_info() {
//     std::stringstream out;
//     out << "Total de mensagens no sistema por tipo: " << std::endl;;
//     for (auto pair : input_info) {
//         out << pair.first << ": " << pair.second << std::endl;
//     }
//     return out;
// }

// void smail::Simulator::generate_output(const std::string& filename = "output.txt") {
//     std::stringstream ss;
//     std::ofstream done;
//     ss << "-- Arquivo de saída gerado por YASIMS (Marleson Graf e Vinicius Freitas [2016])   --" << "\n";
//     ss << "------------------------------------------------------------------------------------" << "\n";
//     ss << "Esse é um relatório de uma simulação rodada com as seguintes especificações:" << "\n";
//     //<< system_specs()
//     ss << "------------------------------------------------------------------------------------" << "\n";
//     ss << "Esses são os resultados requeridos por esta implementação: " << "\n";
//     ss << "Tempo total de simulação: " << clock << " segundos" << "\n";
//        // << "Número de mensagens no sistema no fim da execução: " << msgs_in_system << " mensagens " << "\n";
//        // << "Mínimo de mensagens no sistema: " << min_msgs_in_system << " mensagens" << "\n";
//        // << "Máximo de mensagens no sistema: " << max_msgs_in_system << " mensagens" << "\n";
//     ss << reveal_input_info();
//     ss << avg_occupation(0);
//     ss << avg_occupation(1);
//     ss << reveal_messages_info();
//     ss << consumer.reveal_info();
//     ss << "-------------------------------- Fim do Relatório -----------------------------------" << "\n";
//     done.open(filename);
//     done << out.rdbuf();
// }

size_t smail::Simulator::get_input_info(Address from, Address to) {
    size_t r = 0;
    for (auto pair : input_info) {
        if (pair.first.from == from && pair.first.to == to) {
            r += pair.second;
        }
    }
    return r;
}