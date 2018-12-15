#include <iostream>
#include <string>
#include <vector>

enum class Direction { UP, RIGHT, DOWN, LEFT };

Direction turn_diag_r(Direction d) // / (slash)
{
    switch (d)
    {
        case Direction::UP   : return Direction::RIGHT;
        case Direction::RIGHT: return Direction::UP;
        case Direction::DOWN : return Direction::LEFT;
        case Direction::LEFT : return Direction::DOWN;
    }
}

Direction turn_diag_l(Direction d) // \ (backslash)
{
    switch (d)
    {
        case Direction::UP   : return Direction::LEFT;
        case Direction::RIGHT: return Direction::DOWN;
        case Direction::DOWN : return Direction::RIGHT;
        case Direction::LEFT : return Direction::UP;
    }
}

enum class Turn { LEFT, STRAIGHT, RIGHT };

Turn operator++(Turn& t, int dummy)
{
    Turn copy = t;

    switch (t)
    {
        case Turn::LEFT    : t = Turn::STRAIGHT; break;
        case Turn::STRAIGHT: t = Turn::RIGHT; break;
        case Turn::RIGHT   : t = Turn::LEFT; break;
    }

    return copy;
}

Direction operator+(Direction d, Turn t)
{
    switch(d)
    {
        case Direction::UP:
            switch (t)
            {
                case Turn::LEFT    : return Direction::LEFT;
                case Turn::STRAIGHT: return Direction::UP;
                case Turn::RIGHT   : return Direction::RIGHT;
            }

        case Direction::RIGHT:
            switch (t)
            {
                case Turn::LEFT    : return Direction::UP;
                case Turn::STRAIGHT: return Direction::RIGHT;
                case Turn::RIGHT   : return Direction::DOWN;
            }

        case Direction::DOWN:
            switch (t)
            {
                case Turn::LEFT    : return Direction::RIGHT;
                case Turn::STRAIGHT: return Direction::DOWN;
                case Turn::RIGHT   : return Direction::LEFT;
            }

        case Direction::LEFT:
            switch (t)
            {
                case Turn::LEFT    : return Direction::DOWN;
                case Turn::STRAIGHT: return Direction::LEFT;
                case Turn::RIGHT   : return Direction::UP;
            }
    }
}

Direction& operator+=(Direction& d, Turn t)
{
    return d = d + t;
}

struct Cart
{
    int id;
    int x;
    int y;

    Direction dir;
    Turn next_turn = Turn::LEFT;
    bool crashed = false;
};

bool in(char field, const std::string& chs)
{
    for (char ch : chs)
        if (ch == field) return true;
    return false;
}

char filler(int x, int y, const std::vector<std::string>& map)
{
    char above = y > 0                 ? map[y - 1][x] : 'X';
    char right = x < map[0].size() - 1 ? map[y][x + 1] : 'X';
    char below = y < map.size() - 1    ? map[y + 1][x] : 'X';
    char left  = x > 0                 ? map[y][x - 1] : 'X';

    if (in(above, "|/\\+") && in(right, "-/\\+") && in(below, "|/\\+") && in(left, "-/\\+")) return '+';
    if (in(above, "|/\\+") && in(below, "|/\\+")) return '|';
    if (in(left , "-/\\+") && in(right, "-/\\+")) return '-';
    if ((in(left, "-+") && in(above, "|+")) || (in(right, "-+") && in(below, "|+"))) return '/';
    if ((in(left, "-+") && in(below, "|+")) || (in(right, "-+") && in(above, "|+"))) return '\\';

    throw "ERROR"; // invalid input
}

void step(Cart& c, std::vector<Cart>& carts, const std::vector<std::string>& map)
{
    if (c.crashed) return;

    switch (c.dir)
    {
        case Direction::UP   : c.y -= 1; break;
        case Direction::RIGHT: c.x += 1; break;
        case Direction::DOWN : c.y += 1; break;
        case Direction::LEFT : c.x -= 1; break;
    }

    auto c2 = std::find_if(std::begin(carts), std::end(carts), [c](Cart c2) {
        return (c2.id != c.id) && !c2.crashed && c2.x == c.x && c2.y == c.y;
    });

    if (c2 != std::end(carts))
    {
        c.crashed = c2->crashed = true;
        return;
    }

    switch (map[c.y][c.x])
    {
        case '\\': c.dir = turn_diag_l(c.dir); break;
        case '/' : c.dir = turn_diag_r(c.dir); break;
        case '+' : c.dir += c.next_turn++;     break;
    }
}

void tick(std::vector<Cart>& carts, std::vector<std::string>& map)
{
    for (Cart& c : carts)
        step(c, carts, map);
}

int main()
{
    std::vector<std::string> map;

    while (true)
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.eof()) break;
        map.push_back(line);
    }

    int height = map.size();
    int width = map[0].size();

    std::vector<Cart> carts;

    int id = 0;
    for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
    {
        switch (map[y][x])
        {
            case '^': carts.push_back({id++, x, y, Direction::UP   }); break;
            case '>': carts.push_back({id++, x, y, Direction::RIGHT}); break;
            case 'v': carts.push_back({id++, x, y, Direction::DOWN }); break;
            case '<': carts.push_back({id++, x, y, Direction::LEFT }); break;
        }
    }

    for (Cart c : carts)
        map[c.y][c.x] = filler(c.x, c.y, map);

    while (true)
    {
        std::sort(std::begin(carts), std::end(carts), [](Cart c1, Cart c2) {
            return !c1.crashed && ((c1.y < c2.y) || (c1.y == c2.y && c1.x < c2.x));
        });

        tick(carts, map);

        auto pred = [](Cart c) { return !c.crashed; };
        int count = std::count_if(std::begin(carts), std::end(carts), pred);

        if (count == 1)
        {
            auto c = std::find_if(std::begin(carts), std::end(carts), pred);
            std::cout << c->x << "," << c->y << std::endl;
            return 0;
        }
    }
}