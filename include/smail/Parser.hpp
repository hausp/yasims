
#ifndef PARSER_HPP
#define PARSER_HPP

#include <array>
#include <regex>
#include <string>
#include "Config.hpp"
#include "random/Types.hpp"
#include "Default.hpp"
#include <iostream>
#include <cassert>

namespace parser {
    struct Match {
        dist::Type type;
        std::vector<double> values;
    };

    using Result = std::pair<Match, bool>;

    struct RawConfig {
        using MMMap = std::unordered_map<smail::Message, Match>;
        using AAMMap = std::array<std::array<Match, 3>, 2>;

        parser::Match seed;
        parser::Match sim_time;
        parser::Match timeout;
        std::array<parser::Match, 2> center_sizes;
        std::array<parser::Match, 2> generations;
        std::array<parser::Match, 2> local_proportions;
        std::array<parser::Match, 2> remote_proportions;
        AAMMap local_weights;
        AAMMap remote_weights;
        MMMap reception_times;
        MMMap local_processing_times;
        MMMap remote_processing_times;
    };

    template<dist::Type T>
    struct Regex;

    template<>
    struct Regex<dist::Type::CONS> {
        static constexpr auto VALUE =
        "^\\s*(\\d*\\.?\\d+)\\s*$";
    };

    template<>
    struct Regex<dist::Type::NORM> {
        static constexpr auto VALUE =
        "^\\s*NORM\\s*\\(\\s*(\\d*\\.?\\d+)\\s*,\\s*(\\d*\\.?\\d+)\\s*\\)\\s*$";
    };

    template<>
    struct Regex<dist::Type::UNIF> {
        static constexpr auto VALUE =
        "^\\s*UNIF\\s*\\(\\s*(\\d*\\.?\\d+)\\s*,\\s*(\\d*\\.?\\d+)\\s*\\)\\s*$";
    };

    template<>
    struct Regex<dist::Type::TRIA> {
        static constexpr auto VALUE =
        "^\\s*TRIA\\s*\\(\\s*(\\d*\\.?\\d+)\\s*,\\s*(\\d*\\.?\\d+)\\s*,\\s*(\\d*\\.?\\d+)\\s*\\)\\s*$";
    };

    template<>
    struct Regex<dist::Type::EXPO> {
        static constexpr auto VALUE =
        "^\\s*EXPO\\s*\\(\\s*(\\d*\\.?\\d+)\\s*\\)\\s*$";
    };


    inline size_t parse_header(const std::string& entry,
                               const std::string& utest,
                               const std::string& ltest) {
        for (size_t i = 0; i < 4; i++) {
            if (entry[i] != utest[i] && entry[i] != ltest[i]) {
                return 0;
            }
        }
        auto start = 0;
        for (size_t i = 4; i < entry.size(); i++) {
            if (entry[i] == ' ' || entry[i] == '\t') {
                continue;
            }
            if (entry[i] == '(') {
                start = i+1;
                break;
            }
        }
        return start;
    }

    inline Result parse_cons(const std::string& entry) {
        auto result = Result{Match{dist::Type::CONS}, false};
        auto space = false;
        auto dot = false;
        for (auto& c : entry) {
            if (space && (c != ' ' && c != '\t')) {
                return result;
            } else if (c == ' ' || c == '\t') {
                space = true;
            } else if (!std::isdigit(c)) {
                if (!dot && c == '.') {
                    dot = true;
                } else {
                    return result;
                }
            }
        }
        auto value = std::stod(entry);
        assert(!std::isnan(value) && !std::isinf(value));
        result.first.values.push_back(value);
        result.second = true;
        return result;
    }

    inline Result parse_norm(const std::string& entry) {
        auto result = Result{Match{dist::Type::NORM}, false};
        if (entry.size() < 9) return result;
        auto start = parse_header(entry, "NORM", "norm");
        if (start == 0) return result;

        auto dot = false;
        auto comma = false;
        auto started = false;
        auto space = false;
        auto buffer = std::string{""};

        for (size_t i = start; i < entry.size(); i++) {
            if (!started && (entry[i] == ' ' || entry[i] == '\t')) {
                continue;
            }
            if (!space && std::isdigit(entry[i])) {
                started = true;
                buffer += entry[i];
            } else if (!space && !dot && entry[i] == '.') {
                started = true;
                dot = true;
                buffer += entry[i];
            } else if (!comma && entry[i] == ',') {
                comma = true;
                started = false;
                space = false;
                dot = false;
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                buffer = "";
            } else if (started && comma && entry[i] == ')'){
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                break;
            } else if (entry[i] == ' ') {
                space = true;
            } else {
                return result;
            }
        }
        result.second = true;
        return result;
    }

    inline Result parse_unif(const std::string& entry) {
        auto result = Result{Match{dist::Type::UNIF}, false};
        if (entry.size() < 9) return result;
        auto start = parse_header(entry, "UNIF", "unif");
        if (start == 0) return result;

        auto dot = false;
        auto comma = false;
        auto started = false;
        auto space = false;
        auto buffer = std::string{""};

        for (size_t i = start; i < entry.size(); i++) {
            if (!started && (entry[i] == ' ' || entry[i] == '\t')) {
                continue;
            }
            if (!space && std::isdigit(entry[i])) {
                started = true;
                buffer += entry[i];
            } else if (!space && !dot && entry[i] == '.') {
                started = true;
                dot = true;
                buffer += entry[i];
            } else if (!comma && entry[i] == ',') {
                comma = true;
                started = false;
                space = false;
                dot = false;
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                buffer = "";
            } else if (started && comma && entry[i] == ')'){
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                break;
            } else if (entry[i] == ' ') {
                space = true;
            } else {
                return result;
            }
        }
        result.second = true;
        return result;
    }

    inline Result parse_tria(const std::string& entry) {
        auto result = Result{Match{dist::Type::TRIA}, false};
        if (entry.size() < 11) return result;
        auto start = parse_header(entry, "TRIA", "tria");
        if (start == 0) return result;
        auto dot = false;
        auto comma1 = false;
        auto comma2 = false;
        auto started = false;
        auto space = false;
        auto buffer = std::string{""};

        for (size_t i = start; i < entry.size(); i++) {
            if (!started && (entry[i] == ' ' || entry[i] == '\t')) {
                continue;
            }
            if (!space && std::isdigit(entry[i])) {
                started = true;
                buffer += entry[i];
            } else if (!space && !dot && entry[i] == '.') {
                started = true;
                dot = true;
                buffer += entry[i];
            } else if (!comma1 && entry[i] == ',') {
                comma1 = true;
                started = false;
                space = false;
                dot = false;
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                buffer = ""; 
            } else if (started && comma1 && !comma2 && entry[i] == ',') {
                comma2 = true;
                started = false;
                space = false;
                dot = false;
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                buffer = "";
            } else if (started && comma1 && comma2 && entry[i] == ')') {
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                break;
            } else if (entry[i] == ' ') {
                space = true;
            } else {
                return result;
            }
        }
        result.second = true;
        return result;
    }

    inline Result parse_expo(const std::string& entry) {
        auto result = Result{Match{dist::Type::EXPO}, false};
        if (entry.size() < 7) return result;
        auto start = parse_header(entry, "EXPO", "expo");
        if (start == 0) return result;

        auto dot = false;
        auto started = false;
        auto space = false;
        auto buffer = std::string{""};

        for (size_t i = start; i < entry.size(); i++) {
            if (!started && (entry[i] == ' ' || entry[i] == '\t')) {
                continue;
            }
            if (!space && std::isdigit(entry[i])) {
                started = true;
                buffer += entry[i];
            } else if (!space && !dot && entry[i] == '.') {
                started = true;
                dot = true;
                buffer += entry[i];
            } else if (started && entry[i] == ')'){
                auto value = std::stod(buffer);
                assert(!std::isnan(value) && !std::isinf(value));
                result.first.values.push_back(value);
                break;
            } else if (entry[i] == ' ') {
                space = true;
            } else {
                return result;
            }
        }
        result.second = true;
        return result;
    }

    inline Result regex_match(const std::string& entry) {
        auto result = Result{Match{}, false};
        auto i = 0;
        for (auto& c : entry) {
            if (c == ' ' || c == '\t') {
                ++i;
                continue;
            }
            if (std::isdigit(c) || c == '.') {
                result = parse_cons(entry.substr(i));
                break;
            }
            if (c == 'N' || c == 'n') {
                result = parse_norm(entry.substr(i));
                break;
            }
            if (c == 'U' || c == 'u') {
                result = parse_unif(entry.substr(i));
                break;
            }
            if (c == 'T' || c == 't') {
                result = parse_tria(entry.substr(i));
                break;
            }
            if (c == 'E' || c == 'e') {
                result = parse_expo(entry.substr(i));
                break;
            }
        }
        return result;
    }

    inline Result s_match(const std::string& entry, const std::string& s) {
        static const auto cons = std::regex{Regex<dist::Type::CONS>::VALUE};
        auto matched = std::make_pair(Match{}, false);
        auto match = std::smatch{};

        if (entry == s) {
            matched.first = {
                dist::Type::CONS,
                {-1}
            };
            matched.second = true;
        } else {
            matched = regex_match(entry);
        }
        // } else if (std::regex_match(entry, match, cons)) {
        //     matched.first = {
        //         dist::Type::CONS,
        //         {std::stod(match[1])}
        //     };
        //     matched.second = true;
        // }
        return matched;
    }

    inline Result match(const std::string& entry) {
        return regex_match(entry);
        // static const auto cons = std::regex{Regex<dist::Type::CONS>::VALUE};
        // static const auto norm = std::regex{Regex<dist::Type::NORM>::VALUE};
        // static const auto unif = std::regex{Regex<dist::Type::UNIF>::VALUE};
        // static const auto tria = std::regex{Regex<dist::Type::TRIA>::VALUE};
        // static const auto expo = std::regex{Regex<dist::Type::EXPO>::VALUE};
        // auto matched = std::make_pair(Match{}, false);
        // auto match = std::smatch{};

        // if (std::regex_match(entry, match, cons)) {
        //     matched.first = {
        //         dist::Type::CONS,
        //         {std::stod(match[1])}
        //     };
        //     matched.second = true;
        // } else if (std::regex_match(entry, match, norm)) {
        //     matched.first = {
        //         dist::Type::NORM,
        //         {std::stod(match[1]), std::stod(match[2])}
        //     };
        //     matched.second = true;
        // } else if (std::regex_match(entry, match, unif)) {
        //     matched.first = {
        //         dist::Type::UNIF,
        //         {std::stod(match[1]), std::stod(match[2])}
        //     };
        //     matched.second = true;
        // } else if (std::regex_match(entry, match, tria)) {
        //     matched.first = {
        //         dist::Type::TRIA,
        //         {std::stod(match[1]),
        //          std::stod(match[2]),
        //          std::stod(match[3])}
        //     };
        //     matched.second = true;
        // } else if (std::regex_match(entry, match, expo)) {
        //     matched.first = {
        //         dist::Type::EXPO,
        //         {std::stod(match[1])}
        //     };
        //     matched.second = true;
        // }
        // return matched;
    }

    inline smail::Config lapidate(RawConfig config) {
        auto seed = config.seed.values[0];
        auto use_random_seed = false;
        if (seed == -1) {
            use_random_seed = true;
        } else {
            dist::Global::SEED = seed;
        }
        auto sim_time = config.sim_time.values[0];
        auto infinite_simulation = sim_time == -1;
        auto timeout = config.sim_time.values[0];
        auto center_capacities = std::array<size_t, 2> {{
            static_cast<size_t>(config.center_sizes[0].values[0]),
            static_cast<size_t>(config.center_sizes[1].values[0])
        }};
        auto arrival_times = std::array<dist::funct<>,2> {{
            dist::type_to_function(
                config.generations[0].type,
                config.generations[0].values
            ),
            dist::type_to_function(
                config.generations[0].type,
                config.generations[1].values
            )
        }};

        auto destinations = std::array<dist::disc<smail::Address>, 2> {{
            dist::disc<smail::Address>{
                {smail::Address::LOCAL, smail::Address::REMOTE},
                {config.local_proportions[0].values[0],
                 config.local_proportions[1].values[0]}
            },
            dist::disc<smail::Address>{
                {smail::Address::LOCAL, smail::Address::REMOTE},
                {config.remote_proportions[0].values[0],
                 config.remote_proportions[1].values[0]}
            }
        }};

        auto status_weights = std::array<smail::AWMap, 2> {{
            smail::AWMap{
                {smail::Address::LOCAL,
                 {config.local_weights[0][0].values[0],
                  config.local_weights[0][1].values[0],
                  config.local_weights[0][2].values[0]}},
                {smail::Address::REMOTE,
                 {config.local_weights[1][0].values[0],
                  config.local_weights[1][1].values[0],
                  config.local_weights[1][2].values[0]}},
            },
            smail::AWMap{
                {smail::Address::LOCAL,
                 {config.remote_weights[0][0].values[0],
                  config.remote_weights[0][1].values[0],
                  config.remote_weights[0][2].values[0]}},
                {smail::Address::REMOTE,
                 {config.remote_weights[1][0].values[0],
                  config.remote_weights[1][1].values[0],
                  config.remote_weights[1][2].values[0]}},
            },
        }};

        auto reception_times = smail::MFMap{};
        for (auto& pair : config.reception_times) {
            reception_times[pair.first] = dist::type_to_function(
                pair.second.type,
                pair.second.values
            );
        }

        auto processing_times = std::array<smail::MFMap, 2>{};
        for (auto& pair : config.local_processing_times) {
            processing_times[0][pair.first] = dist::type_to_function(
                pair.second.type,
                pair.second.values
            );
        }

        for (auto& pair : config.remote_processing_times) {
            processing_times[1][pair.first] = dist::type_to_function(
                pair.second.type,
                pair.second.values
            );
        }

        return smail::Config{
            use_random_seed,
            infinite_simulation,
            sim_time,
            timeout,
            center_capacities,
            std::move(arrival_times),
            std::move(destinations),
            std::move(status_weights),
            std::move(reception_times),
            std::move(processing_times)
        };
    }
}

#endif /* PARSER_HPP */
