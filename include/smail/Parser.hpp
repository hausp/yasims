
#ifndef PARSER_HPP
#define PARSER_HPP

#include <array>
#include <regex>
#include <string>
#include "Config.hpp"
#include "random/Types.hpp"
#include "Default.hpp"

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
        } else if (std::regex_match(entry, match, cons)) {
            matched.first = {
                dist::Type::CONS,
                {std::stod(match[1])}
            };
            matched.second = true;
        }
        return matched;
    }

    inline Result match(const std::string& entry) {
        static const auto cons = std::regex{Regex<dist::Type::CONS>::VALUE};
        static const auto norm = std::regex{Regex<dist::Type::NORM>::VALUE};
        static const auto unif = std::regex{Regex<dist::Type::UNIF>::VALUE};
        static const auto tria = std::regex{Regex<dist::Type::TRIA>::VALUE};
        static const auto expo = std::regex{Regex<dist::Type::EXPO>::VALUE};
        auto matched = std::make_pair(Match{}, false);
        auto match = std::smatch{};

        if (std::regex_match(entry, match, cons)) {
            matched.first = {
                dist::Type::CONS,
                {std::stod(match[1])}
            };
            matched.second = true;
        } else if (std::regex_match(entry, match, norm)) {
            matched.first = {
                dist::Type::NORM,
                {std::stod(match[1]), std::stod(match[2])}
            };
            matched.second = true;
        } else if (std::regex_match(entry, match, unif)) {
            matched.first = {
                dist::Type::UNIF,
                {std::stod(match[1]), std::stod(match[2])}
            };
            matched.second = true;
        } else if (std::regex_match(entry, match, tria)) {
            matched.first = {
                dist::Type::TRIA,
                {std::stod(match[1]),
                 std::stod(match[2]),
                 std::stod(match[3])}
            };
            matched.second = true;
        } else if (std::regex_match(entry, match, expo)) {
            matched.first = {
                dist::Type::EXPO,
                {std::stod(match[1])}
            };
            matched.second = true;
        }
        return matched;
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
        auto center_capacities = std::array<size_t, 2>{
            static_cast<size_t>(config.center_sizes[0].values[0]),
            static_cast<size_t>(config.center_sizes[1].values[0])
        };
        auto arrival_times = std::array<dist::funct<>,2> {
            dist::type_to_function(
                config.generations[0].type,
                config.generations[0].values
            ),
            dist::type_to_function(
                config.generations[0].type,
                config.generations[1].values
            )
        };

        auto destinations = std::array<dist::disc<smail::Address>, 2> {
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
        };

        auto status_weights = std::array<smail::AWMap, 2> {
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
        };

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
