#include <array>
#include <iostream>
#include <limits>

constexpr int SERIAL = 1133;
constexpr int GRIDSIZE = 300;

int power(int x, int y, int serial)
{
    return ((((x + 10) * y + serial) * (x + 10)) / 100) % 10 - 5;
}

int main()
{
    std::array<std::array<int, GRIDSIZE + 1>, GRIDSIZE + 1> grid;

    for (int x = 1; x <= GRIDSIZE; ++x)
    for (int y = 1; y <= GRIDSIZE; ++y)
        grid[x][y] = power(x, y, SERIAL);

    int max_power = std::numeric_limits<int>::min();
    int maxx, maxy, maxsize;

    for (int size = 1; size <= GRIDSIZE; ++size)
    for (int x = 1; x <= GRIDSIZE - size + 1; ++x)
    for (int y = 1; y <= GRIDSIZE - size + 1; ++y)
    {
        int p = 0;
        for (int dx = 0; dx < size; ++dx)
        for (int dy = 0; dy < size; ++dy)
            p += grid[x + dx][y + dy];

        if (p > max_power)
        {
            max_power = p;
            maxx = x; maxy = y; maxsize = size;
        }
    }

    std::cout << maxx << "," << maxy << "," << maxsize << std::endl;

    return 0;
}