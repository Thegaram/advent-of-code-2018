#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

constexpr int MAX_DIST = 10000;

struct Coords
{
    int x;
    int y;
};

int abs(int x)
{
    return x >= 0 ? x : -x;
}

int distance(Coords c1, Coords c2)
{
    return abs(c1.x - c2.x) + abs(c1.y - c2.y);
}

class Field
{
private:
    static constexpr int DEF = -1;

    int min_x, min_y, max_x, max_y;
    std::vector<std::vector<int>> field;

public:
    Field(int min_x, int min_y, int max_x, int max_y):
        min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y)
    {
        int width = max_x - min_x + 1;
        int height = max_y - min_y + 1;
        field = std::vector<std::vector<int>>(width, std::vector<int>(height, DEF));
    }

    int at(int x, int y)
    {
        if (x < min_x || x > max_x || y < min_y || y > max_y) return DEF;
        return field[x - min_x][y - min_y];
    }

    void set(int x, int y, int ch)
    {
        assert(x >= min_x && x <= max_x);
        assert(y >= min_y && y <= max_y);
        field[x - min_x][y - min_y] = ch;
    }
};

int main()
{
    // std::vector<Coords> coordinates;
    std::map<int, Coords> coordinates;

    int min_x, min_y, max_x, max_y;
    min_x = min_y = std::numeric_limits<int>::max();
    max_x = max_y = std::numeric_limits<int>::min();

    int id = 0;

    while (true)
    {
        int x, y; char dummy;
        std::cin >> x >> dummy >> y;
        if (std::cin.eof()) break;

        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;

        coordinates[id++] = {x, y};
    }

    int cnt = 0;
    for (int x = min_x; x <= max_x; ++x)
    for (int y = min_y; y <= max_y; ++y)
    {
        int dist_sum = 0;
        for (auto entry : coordinates)
            dist_sum += distance(entry.second, {x, y});

        if (dist_sum < MAX_DIST)
            ++cnt;
    }

    std::cout << "result: " << cnt << std::endl;

    return 0;
}