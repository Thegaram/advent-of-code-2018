#include <cassert>
#include <iostream>
#include <string>
#include <regex>
#include <array>

constexpr int DIM = 1000;

int main()
{
    std::array<std::array<int, DIM>, DIM> fabric = {};
    std::regex regex {R"(^#(\d+) @ (\d+),(\d+): (\d+)x(\d+)$)"};

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

            for (int row = 0; row < height; ++row)
            for (int col = 0; col < width; ++col)
            {
                ++fabric[offset_v + row][offset_h + col];
            }
        }
    }

    int count = 0;
    for (int row = 0; row < DIM; ++row)
    for (int col = 0; col < DIM; ++col)
    {
        if (fabric[row][col] > 1)
            ++count;
    }

    std::cout << "result = " << count << std::endl;

    return 0;
}