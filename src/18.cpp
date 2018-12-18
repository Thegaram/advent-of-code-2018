#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

constexpr int MINUTES = 10;

constexpr char OPEN = '.';
constexpr char TREE = '|';
constexpr char LUMBER = '#';

struct Coord
{
    int x, y;
};

char new_value(const std::vector<std::string>& map, Coord s)
{
    int height = map.size();
    int width = map[0].size();

    std::vector<Coord> adj;
    if (s.x > 0 && s.y > 0)              adj.push_back({s.x - 1, s.y - 1});
    if (s.y > 0)                         adj.push_back({s.x    , s.y - 1});
    if (s.x < width - 1 && s.y > 0)      adj.push_back({s.x + 1, s.y - 1});
    if (s.x > 0)                         adj.push_back({s.x - 1, s.y    });
    if (s.x < width - 1)                 adj.push_back({s.x + 1, s.y    });
    if (s.x > 0 && s.y < height)         adj.push_back({s.x - 1, s.y + 1});
    if (s.y < height)                    adj.push_back({s.x    , s.y + 1});
    if (s.x < width - 1 && s.y < height) adj.push_back({s.x + 1, s.y + 1});

    auto b = std::begin(adj), e = std::end(adj);
    int num_trees  = std::count_if(b, e, [&map](Coord c) { return map[c.y][c.x] == TREE; });
    int num_lumber = std::count_if(b, e, [&map](Coord c) { return map[c.y][c.x] == LUMBER; });

    char current = map[s.y][s.x];
    char result = current;

    switch (current)
    {
        case OPEN:   if (num_trees >= 3) result = TREE; break;
        case TREE:   if (num_lumber >= 3) result = LUMBER; break;
        case LUMBER: if (num_lumber == 0 || num_trees == 0) result = OPEN; break;
    }

    return result;
}

void transform(const std::vector<std::string>& from, std::vector<std::string>& to)
{
    int height = from.size();
    int width = from[0].size();

    assert(height == to.size());
    assert(width == to[0].size());

    for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
        to[y][x] = new_value(from, {x, y});
}

int resource_value(const std::vector<std::string>& map)
{
    int height = map.size();
    int width = map[0].size();

    int num_trees = 0;
    int num_lumber = 0;

    for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
    {
        if (map[y][x] == TREE)   ++ num_trees;
        if (map[y][x] == LUMBER) ++ num_lumber;
    }

    return num_trees * num_lumber;
}

int main()
{
    std::vector<std::string> map;

    while (true)
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.eof() || line == "") break;
        map.push_back(line);
    }

    int height = map.size();
    int width = map[0].size();

    std::vector<std::string> tmp_map = std::vector<std::string>(height, std::string(width, OPEN));

    int minute = 0;

    while (minute++ < MINUTES)
    {
        transform(map, tmp_map);
        std::swap(map, tmp_map);
    }

    std::cout << "result = " << resource_value(map) << std::endl;

    return 0;
}