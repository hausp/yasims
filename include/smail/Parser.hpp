
#ifndef PARSER_HPP
#define PARSER_HPP

#include <regex>
#include <string>
#include "random/Types.hpp"

// NORM,   // Normal distribution
// UNIF,   // Uniform distribution
// TRIA,   // Triangular distribution
// EXPO,   // Exponential distribution
// DISC,   // Discrete distribution
// CONS,   // Constant distribution

namespace parser {
    struct Match {
        dist::Type type;
        std::vector<double> values;
    };

    template<dist::Type T>
    struct Regex;

    template<>
    struct Regex<dist::Type::CONS> {
        static constexpr auto VALUE =
        "^\\s*\\d*\\.?\\d+\\s*$";
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
        "^\\s*EXPO\\s*\\(\\s*(\\d*\\.?\\d+)\\s*,\\s*(\\d*\\.?\\d+)\\s*\\)\\s*$";
    };

    inline std::pair<Match, bool> match(const std::string& entry) {
        auto cons = std::regex{Regex<dist::Type::CONS>::VALUE};
        auto norm = std::regex{Regex<dist::Type::NORM>::VALUE};
        auto unif = std::regex{Regex<dist::Type::UNIF>::VALUE};
        auto tria = std::regex{Regex<dist::Type::TRIA>::VALUE};
        auto expo = std::regex{Regex<dist::Type::EXPO>::VALUE};
        std::smatch match;
        auto matched = std::make_pair(Match{}, false);

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
        }

        if (std::regex_match(entry, match, expo)) {
            matched.first = {
                dist::Type::EXPO,
                {std::stod(match[1]), std::stod(match[2])}
            };
            matched.second = true;
        }
        return matched;
    }

}

#endif
