#include <iostream>
#include <iterator>
#include <list>
#include <vector>

constexpr int NUMPLAYERS = 428;
constexpr int LAST = 7082500;

template <typename I>
void increase_cyclical(I& iter, const I begin, const I end)
{
    if (iter == end)
        iter = begin;
    iter++;
}

template <typename I>
void decrease_cyclical(I& iter, const I begin, const I end)
{
    if (iter == begin)
        iter = end;
    iter--;
}

int main()
{
    std::vector<long> scores(NUMPLAYERS, 0);
    std::list<int> marbles = {0};

    auto location = std::cbegin(marbles);
    int marble = 1;
    int player = 1;

    while (marble <= LAST)
    {
        auto begin = std::cbegin(marbles);
        auto end = std::cend(marbles);

        if (marble % 23 == 0)
        {
            for (int ii = 0; ii < 7; ++ii)
                decrease_cyclical(location, begin, end);

            scores[player] += marble + *location;
            location = marbles.erase(location);
        }
        else
        {
            increase_cyclical(location, begin, end);
            increase_cyclical(location, begin, end);
            location = marbles.insert(location, marble);
        }

        marble++;
        player = (player + 1) % NUMPLAYERS;
    }

    long max = 0;
    for (long score : scores)
        if (score > max)
            max = score;

    std::cout << "high score is " << max << std::endl;

    return 0;
}