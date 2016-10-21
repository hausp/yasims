
#ifndef PARSER_HPP
#define PARSER_HPP

#include <regex>
#include <string>
#include "random/Types.hpp"
#include "Default.hpp"

namespace parser {
    struct Match {
        dist::Type type;
        std::vector<double> values;
    };

    using Result = std::pair<Match, bool>;

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

}

#endif /* PARSER_HPP */
