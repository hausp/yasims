
#include <cassert>
#include "interface/GTKInterface.hpp"
#include "smail/Simulator.hpp"
#include "random/Function.hpp"
#include "wrapper/Signal.hpp"
#include "core/Context.hpp"
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
    initialize_input_info();
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
    if (stopped) {
        stopped = false;
        reset();
    }

    setup();

    animated = animate;
    execute = true;

    if (animate) {
        launch_gtk_function();
    } else {
        // launch_gtk_function_faster();
        std::lock_guard<std::mutex> guard{mutex};
        cv.notify_all();
    }
}

void smail::Simulator::step() {
    if (execute) return;
    if (stopped) {
        stopped = false;
        reset();
    }

    animated = true;
    setup();
    simulate();
    show_statistics();
    animated = false;
}

void smail::Simulator::pause() {
    if (!animated) {
        std::lock_guard<std::mutex> guard{mutex_aux};
    }

    execute = false;

    show_statistics();
}

void smail::Simulator::stop() {
    if (!animated) {
        std::lock_guard<std::mutex> guard{mutex_aux};
    }

    execute = false;
    stopped = true;

    show_statistics();
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
            auto lock = std::unique_lock<std::mutex>{mutex_aux};
            lock.unlock();
            // std::this_thread::sleep_for(std::chrono::nanoseconds(50));
            // std::this_thread::yield();
            simulate();
        }

    }
}

// For animations, better be in gtk's thread
bool smail::Simulator::gtk_run() {
    simulate();
    if (animated) {
        show_statistics();
    }
    return execute;
}

void smail::Simulator::simulate() { 
    // Get next event
    auto e = events.top();
    // Update simulation clock and statistics
    update(e.time);
    // Execute event action and check if successful
    if (e.action()) {
        // Successful events are removed
        events.pop();
    }
    // Update execute control variable
    auto terminated = events.empty();
    if (!config.infinite_simulation) {
        terminated = terminated || clock >= config.sim_time;
    }
    if (terminated) {
        g_timeout_add_full(
            G_PRIORITY_DEFAULT + 10,
            100,
            (GSourceFunc)
            (aw::Signal<Context>::function<bool()>
                ::callback<&Context::stop_gui_behaviour>),
            nullptr,
            nullptr
        );
        if (!animated) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    stopped = stopped || terminated;
    execute = execute && !stopped;
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
    clock = 0;
    
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

    initialize_input_info();
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

void smail::Simulator::launch_gtk_function_faster() {
    gtk_handler = g_timeout_add_full(
        G_PRIORITY_HIGH,
        1,
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
    config = std::move(c);
    reset();
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
    auto faster_guy = std::string{};
    auto faster_guy_speed = consumer.get_faster();
    if (faster_guy_speed == std::numeric_limits<double>::max()) {
        faster_guy = "infinite";
    } else {
        faster_guy = std::to_string(faster_guy_speed);
    }
    auto llcount = get_input_info(Address::LOCAL, Address::LOCAL);
    auto lrcount = get_input_info(Address::LOCAL, Address::REMOTE);
    auto rlcount = get_input_info(Address::REMOTE, Address::LOCAL);
    auto rrcount = get_input_info(Address::REMOTE, Address::REMOTE);

    auto statistics = std::array<std::string, 44> {
        std::to_string(clock),
        std::to_string(msgs_in_system),
        std::to_string(min_msgs_in_system),
        std::to_string(max_msgs_in_system),
        std::to_string(system_occupation),
        std::to_string(llcount + lrcount + rlcount + rrcount),
        std::to_string(llcount),
        std::to_string(lrcount),
        std::to_string(rlcount),
        std::to_string(rrcount),
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
        faster_guy,
        std::to_string(consumer.get_slower()),
        std::to_string(consumer.get_avg_sys_time())
    };
    aw::Signal<GTKInterface>::function<void(std::array<std::string, 44>)>
        ::callback<&GTKInterface::show_statistics>(std::move(statistics));
}

void smail::Simulator::initialize_input_info() {
    input_info[(Message{Address::LOCAL, Address::LOCAL, Status::SUCCESS})] = 0;
    input_info[(Message{Address::LOCAL, Address::LOCAL, Status::FAILURE})] = 0;
    input_info[(Message{Address::LOCAL, Address::LOCAL, Status::POSTPONED})] = 0;
    input_info[(Message{Address::LOCAL, Address::REMOTE, Status::SUCCESS})] = 0;
    input_info[(Message{Address::LOCAL, Address::REMOTE, Status::FAILURE})] = 0;
    input_info[(Message{Address::LOCAL, Address::REMOTE, Status::POSTPONED})] = 0;
    input_info[(Message{Address::REMOTE, Address::LOCAL, Status::SUCCESS})] = 0;
    input_info[(Message{Address::REMOTE, Address::LOCAL, Status::FAILURE})] = 0;
    input_info[(Message{Address::REMOTE, Address::LOCAL, Status::POSTPONED})] = 0;
    input_info[(Message{Address::REMOTE, Address::REMOTE, Status::SUCCESS})] = 0;
    input_info[(Message{Address::REMOTE, Address::REMOTE, Status::FAILURE})] = 0;
    input_info[(Message{Address::REMOTE, Address::REMOTE, Status::POSTPONED})] = 0;
}

std::string smail::Simulator::reveal_messages_info() {
    std::string out = "";
    out += "Mensagens restantes nos sistema: ";
    out += std::to_string(msgs_in_system);
    out += " mensagens\n";
    out += "Máximo de mensagens no sistema: ";
    out += std::to_string(max_msgs_in_system);
    out += " mensagens\n";
    out += "Mínimo de mensagens no sistema: ";
    out += std::to_string(min_msgs_in_system);
    out += " mensagens\n";
    out += "Número médio de mensagens no sistema: ";
    out += std::to_string(system_occupation);
    out += "\n";
    return out;
 }

std::string smail::Simulator::avg_occupation(size_t index) {
    std::string out = "";
    out += "A ocupação média no centro ";
    out += std::to_string(index+1) ;
    out += " foi: "; 
    out += std::to_string(occupation_on_centers[index]);
    out += "\n";
    out += "E o tamanho médio da fila foi: "; 
    out += std::to_string(occupation_on_queues[index]);
    out += "\n";
    return out;
}

std::string smail::Simulator::reveal_input_info() {
    std::string out = "";
    out += "Total de mensagens no sistema por tipo: \n";
    for (auto pair : input_info) {
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

void smail::Simulator::generate_output(const std::string& filename) {
    std::string ss = "";
    std::ofstream done;
    ss += "-- Arquivo de saída gerado por YASIMS (Marleson Graf e Vinicius Freitas [2016])   --\n";
    ss += "------------------------------------------------------------------------------------\n";
    //ss += "Esse é um relatório de uma simulação rodada com as seguintes especificações:\n";
    //<< system_specs()
    ss += "------------------------------------------------------------------------------------\n";
    ss += "Esses são os resultados requeridos por esta implementação: \n";
    ss += "Tempo total de simulação: ";
    ss += std::to_string(clock);
    ss += " segundos\n";
      //  << "Número de mensagens no sistema no fim da execução: " << msgs_in_system << " mensagens " << "\n";
        //<< "Mínimo de mensagens no sistema: " << min_msgs_in_system << " mensagens" << "\n";
       // << "Máximo de mensagens no sistema: " << max_msgs_in_system << " mensagens" << "\n";
    ss += reveal_input_info();
    ss += avg_occupation(0);
    ss += avg_occupation(1);
    ss += reveal_messages_info();
    ss += consumer.reveal_info();
    ss += "-------------------------------- Fim do Relatório -----------------------------------\n";
    done.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    done << ss;
}

size_t smail::Simulator::get_input_info(Address from, Address to) {
    size_t r = 0;
    for (auto pair : input_info) {
        if (pair.first.from == from && pair.first.to == to) {
            r += pair.second;
        }
    }
    return r;
}