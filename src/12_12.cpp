#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

constexpr long GENERATIONS = 20; // 50000000000
constexpr int BUFFER = 30;

struct Rule
{
    const std::string source;
    const char result;
};

std::optional<std::string> read_state(std::istream& is)
{
    static const std::regex regex {R"(^initial state: ([#.]+)$)"};

    if (std::cin.eof())
        return {};

    std::string state;
    std::getline(std::cin, state);

    std::smatch matches;
    if (!std::regex_search(state, matches, regex))
        return {};

    return matches[1];
}

std::optional<Rule> read_rule(std::istream& is)
{
    static const std::regex regex {R"(^([#.]{5}) => ([#.])$)"};

    if (is.eof())
        return {};

    std::string line;
    std::getline(is, line);

    std::smatch matches;
    if (!std::regex_search(line, matches, regex))
        return {};

    auto source = matches[1];
    char result = matches[2].str()[0];

    return Rule {source, result};
}

std::string transform(const std::string& from, const std::unordered_map<std::string, char>& rules)
{
    std::string result(from.size(), '.');

    for (int ii = 0; ii <= from.size() - 5; ++ii)
    {
        std::string key = from.substr(ii, 5);
        result[ii + 2] = (rules.count(key) > 0) ? rules.at(key) : '.';
    }

    return result;
}

long calculate_sum(const std::string& state)
{
    long sum = 0;
    for (long ii = 0; ii < state.size(); ++ii)
    {
        long pot = ii - BUFFER;
        if (state[ii] == '#') sum += pot;
    }
    return sum;
}

int main()
{
    auto state_opt = read_state(std::cin);
    if (!state_opt)
    {
        std::cerr << "Unable to read initial state" << std::endl;
        return -1;
    }

    std::string state = std::string(BUFFER, '.') + *state_opt + std::string(BUFFER, '.');

    std::string dummy;
    std::getline(std::cin, dummy);

    std::unordered_map<std::string, char> rules;

    while (true)
    {
        auto rule = read_rule(std::cin);
        if (!rule) break;
        rules[rule->source] = rule->result;
    }

    for (long ii = 1; ii <= GENERATIONS; ++ii)
    {
        state = transform(state, rules);
        // std::cout << "sum = " << calculate_sum(state) << std::endl;
        // 1000000000508
    }

    std::cout << "sum = " << calculate_sum(state) << std::endl;
}