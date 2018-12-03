#include <cassert>
#include <iostream>
#include <string>
#include <regex>
#include <array>

constexpr int DIM = 1000;

struct Claim
{
    int id;
    int offset_h;
    int offset_v;
    int width;
    int height;
};

int main()
{
    std::array<std::array<int, DIM>, DIM> fabric = {};
    std::regex regex {R"(^#(\d+) @ (\d+),(\d+): (\d+)x(\d+)$)"};

    std::vector<Claim> claims;

    while (true)
    {
        std::string current;
        std::getline(std::cin, current);
        if (std::cin.eof()) break;

        std::smatch matches;
        if (std::regex_search(current, matches, regex))
        {
            assert(matches.size() == 6);

            int id       = std::stoi(matches[1]);
            int offset_h = std::stoi(matches[2]);
            int offset_v = std::stoi(matches[3]);
            int width    = std::stoi(matches[4]);
            int height   = std::stoi(matches[5]);

            claims.push_back({id, offset_h, offset_v, width, height});
        }
    }

    for (Claim claim : claims)
    {
        for (int row = 0; row < claim.height; ++row)
        for (int col = 0; col < claim.width; ++col)
        {
            ++fabric[claim.offset_v + row][claim.offset_h + col];
        }
    }

    for (Claim claim : claims)
    {
        bool overlap = false;

        for (int row = 0; row < claim.height; ++row)
        for (int col = 0; col < claim.width; ++col)
        {
            if (fabric[claim.offset_v + row][claim.offset_h + col] > 1)
                overlap = true;
        }

        if (!overlap)
            std::cout << "Claim without overlap: #" << claim.id << std::endl;
    }

    return 0;
}