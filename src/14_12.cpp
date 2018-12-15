#include <iostream>
#include <numeric>
#include <vector>

template <typename T, typename F>
void for_each_digit(T n, F f)
{
    if (n < 10) return f(n);
    for_each_digit(n / 10, f);
    f(n % 10);
}

int puzzle1(int input)
{
    std::vector<int> recipes = {3, 7};
    recipes.reserve(input + 10);

    std::size_t first = 0;
    std::size_t second = 1;

    while (recipes.size() < (input + 10))
    {
        int sum = recipes[first] + recipes[second];
        for_each_digit(sum, [&recipes](int d){
            recipes.push_back(d);
        });

        first = (first + recipes[first] + 1) % recipes.size();
        second = (second + recipes[second] + 1) % recipes.size();
    }

    return std::accumulate(std::begin(recipes) + input, std::begin(recipes) + input + 10, 0, [](int acc, int next){
        return 10 * acc + next;
    });
}

int puzzle2(const std::vector<int>& input)
{
    std::vector<int> recipes = {3, 7};

    std::size_t first = 0;
    std::size_t second = 1;

    while (true)
    {
        int sum = recipes[first] + recipes[second];
        int numdigits = 0;
        for_each_digit(sum, [&recipes, &numdigits](int d){
            recipes.push_back(d);
            ++numdigits;
        });

        first = (first + recipes[first] + 1) % recipes.size();
        second = (second + recipes[second] + 1) % recipes.size();

        if (recipes.size() < input.size() + numdigits) continue;

        for (std::size_t ii = recipes.size() - input.size() - numdigits; ii < recipes.size() - input.size(); ++ii)
        {
            bool matching = true;

            for (std::size_t jj = 0; jj < input.size(); ++jj)
            {
                if (input[jj] != recipes[ii + jj])
                {
                    matching = false;
                    break;
                }
            }

            if (matching)
                return ii;
        }
    }
}

int main()
{
    std::cout << puzzle1(681901) << std::endl;
    std::cout << puzzle2({6,8,1,9,0,1}) << std::endl;

    return 0;
}