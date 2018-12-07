#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

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

    Field field(min_x, min_y, max_x, max_y);

    for (int x = min_x; x <= max_x; ++x)
    for (int y = min_y; y <= max_y; ++y)
    {
        int min_distance = std::numeric_limits<int>::max();
        char min_id = -1;

        for (auto entry : coordinates)
        {
            int id = entry.first;
            Coords c = entry.second;

            if (c.x == x && c.y == y)
            {
                min_id = id;
                break;
            }

            int dist = distance(c, {x, y});
            if (dist < min_distance)
            {
                min_distance = dist;
                min_id = id;
            }
            else if (dist == min_distance)
            {
                min_id = -2;
            }
        }

        assert(min_id != -1);
        field.set(x, y, min_id);
    }

    constexpr int FRAME = 100;

    for (int y = min_y - FRAME; y <= max_y + FRAME; ++y)
    {
        for (int x = min_x - FRAME; x <= max_x + FRAME; ++x)
        {
            std::cout << std::setw(2) << field.at(x, y) << " ";
        }
        std::cout << std::endl;
    }

    for (auto entry : coordinates)
    {
        int id = entry.first;
        int count = 0;

        for (int x = min_x; x <= max_x; ++x)
        for (int y = min_y; y <= max_y; ++y)
        {
            if (field.at(x, y) == id)
                ++count;
        }

        std::cout << id << ": " << count << std::endl;
    }

    return 0;
}