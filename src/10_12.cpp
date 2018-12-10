// g++ --std=c++17 -Wall src/10_12.cpp -lSDL2

#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 200;
constexpr int POINT_SIZE = 16;

struct Point
{
    const int x;
    const int y;
    const int vx;
    const int vy;
};

void find_bounds(const std::vector<Point>& points, int t, int& minx, int& miny, int& maxx, int& maxy)
{
    minx = miny = std::numeric_limits<int>::max();
    maxx = maxy = std::numeric_limits<int>::min();

    for (Point p : points)
    {
        int x = p.x + t * p.vx;
        int y = p.y + t * p.vy;

        if (x < minx) minx = x;
        if (y < miny) miny = y;
        if (x > maxx) maxx = x;
        if (y > maxy) maxy = y;
    }
}

int bounding_box(const std::vector<Point>& points, int t)
{
    int minx, miny, maxx, maxy;
    find_bounds(points, t, minx, miny, maxx, maxy);
    return (maxx - minx) + (maxy - miny);
}

void render_points(const std::vector<Point>& points, int t, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int minx, miny, maxx, maxy;
    find_bounds(points, t, minx, miny, maxx, maxy);

    for (Point p : points)
    {
        int x = p.x + t * p.vx;
        int y = p.y + t * p.vy;

        int pixel_x = (x - minx) / (double) (maxx - minx) * WINDOW_WIDTH;
        int pixel_y = (y - miny) / (double) (maxy - miny) * WINDOW_HEIGHT;

        for (int dx = -POINT_SIZE / 2; dx <= POINT_SIZE / 2; ++dx)
        for (int dy = -POINT_SIZE / 2; dy <= POINT_SIZE / 2; ++dy)
            SDL_RenderDrawPoint(renderer, pixel_x + dx, pixel_y + dy);
    }

    SDL_RenderPresent(renderer);
}

int main()
{
    std::regex regex {R"(^position=<\s*(-?\d+),\s*(-?\d+)>\s+velocity=<\s*(-?\d+),\s*(-?\d+)>$)"};

    std::vector<Point> points;

    // read input
    while (true)
    {
        std::string current;
        std::getline(std::cin, current);
        if (std::cin.eof()) break;

        std::smatch matches;
        if (std::regex_search(current, matches, regex))
        {
            int x  = std::stoi(matches[1]);
            int y  = std::stoi(matches[2]);
            int vx = std::stoi(matches[3]);
            int vy = std::stoi(matches[4]);

            points.push_back({x, y, vx, vy});
        }
    }

    // find time point with minimum bounding box size
    int t = 0;
    while (bounding_box(points, t) > bounding_box(points, t + 1)) ++t;
    std::cout << "t = " << t << std::endl;

    // render
    SDL_Renderer* renderer;
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    render_points(points, t, renderer);

    SDL_Event event;
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}