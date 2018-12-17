#include <cassert>
#include <iostream>
#include <limits>
#include <regex>
#include <vector>

constexpr char SAND = '.';
constexpr char CLAY = '#';
constexpr char SPRING = '+';
constexpr char STEADY_WATER = '~';
constexpr char MOVING_WATER = '|';

constexpr int SPRING_X = 500;
constexpr int SPRING_y = 0;

struct Range
{
    int x0, x1, y0, y1;
};

struct Coord
{
    int x, y;
};

void convert_water(std::vector<std::string>& map, Coord s)
{
    int height = map.size();
    int width  = map[0].size();

    assert(s.x >= 0 && s.x < width);
    assert(s.y < height);

    bool contained_left = false;
    bool contained_right = false;

    for (int x = s.x; x >= 0; --x)
    {
        if (map[s.y][x] == CLAY) { contained_left = true; break; }
        if (map[s.y][x] == SAND) { break; }
        if (s.y + 1 < height && map[s.y + 1][x] == MOVING_WATER) { break; }
        assert(map[s.y][x] == MOVING_WATER);
    }

    for (int x = s.x; x < width; ++x)
    {
        if (map[s.y][x] == CLAY) { contained_right = true; break; }
        if (map[s.y][x] == SAND) { break; }
        if (s.y + 1 < height && map[s.y + 1][x] == MOVING_WATER) { break; }
        assert(map[s.y][x] == MOVING_WATER);
    }

    if (!contained_left || !contained_right)
        return;

    for (int x = s.x; x >= 0; --x)
    {
        if (map[s.y][x] == SAND) break;
        if (map[s.y][x] == CLAY) break;
        assert(map[s.y][x] == MOVING_WATER || map[s.y][x] == STEADY_WATER);
        map[s.y][x] = STEADY_WATER;
    }

    for (int x = s.x; x < width; ++x)
    {
        if (map[s.y][x] == SAND) break;
        if (map[s.y][x] == CLAY) break;
        assert(map[s.y][x] == MOVING_WATER || map[s.y][x] == STEADY_WATER);
        map[s.y][x] = STEADY_WATER;
    }
}

void traverse(std::vector<std::string>& map, Coord s)
{
    int height = map.size();
    int width  = map[0].size();

    assert(s.x >= 0 && s.x < width);

    map[s.y][s.x] = MOVING_WATER;

    if (s.y == height - 1) return;

    const char& left  = map[s.y][s.x - 1];
    const char& right = map[s.y][s.x + 1];
    const char& below = map[s.y + 1][s.x];

    if (below == SAND)
    {
        traverse(map, {s.x, s.y + 1});
        convert_water(map, {s.x, s.y + 1});
    }

    if ((below == CLAY || below == STEADY_WATER) && left == SAND)
        traverse(map, {s.x - 1, s.y});

    if ((below == CLAY || below == STEADY_WATER) && right == SAND)
        traverse(map, {s.x + 1, s.y});
}

std::vector<Range> read_ranges(int& minx, int& miny, int& maxx, int& maxy)
{
    std::regex regex {R"(^(x|y)=(\d+), (x|y)=(\d+)\.\.(\d+)$)"};

    minx = std::numeric_limits<int>::max(), miny = 0;
    maxx = maxy = std::numeric_limits<int>::min();

    std::vector<Range> ranges;

    while (true)
    {
        std::string current;
        std::getline(std::cin, current);
        if (std::cin.eof()) break;

        std::smatch m;
        if (!std::regex_search(current, m, regex))
            break;

        char c1 = m[1].str()[0]; int from1 = std::stoi(m[2]); int to1 = from1;
        char c2 = m[3].str()[0]; int from2 = std::stoi(m[4]); int to2 = std::stoi(m[5]);

        assert ((c1 == 'x' && c2 == 'y') || (c1 == 'y' && c2 == 'x'));

        int x0, x1, y0, y1;
        if (c1 == 'x') { x0 = from1; x1 = to1; y0 = from2; y1 = to2; }
        if (c1 == 'y') { x0 = from2; x1 = to2; y0 = from1; y1 = to1; }

        if (x0 < minx) minx = x0;
        if (x1 > maxx) maxx = x1;
        if (y1 > maxy) maxy = y1;

        ranges.push_back({x0, x1, y0, y1});
    }

    return ranges;
}

int main()
{
    int minx, miny, maxx, maxy;
    std::vector<Range> ranges = read_ranges(minx, miny, maxx, maxy);

    minx--; maxx++;
    int height = maxy - miny + 1;
    int width  = maxx - minx + 1;
    std::vector<std::string> map (height, std::string(width, SAND));

    for (auto r : ranges)
    for (int y = r.y0; y <= r.y1; ++y)
    for (int x = r.x0; x <= r.x1; ++x)
        map[y - miny][x - minx] = CLAY;

    map[SPRING_y - miny][SPRING_X - minx] = SPRING;

    traverse(map, {SPRING_X - minx, SPRING_y - miny + 1});

    int count = 0;
    for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
        if (map[y][x] == STEADY_WATER) ++count;

    std::cout << "count = " << count << std::endl;
    return 0;
}
