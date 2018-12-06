#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <limits>

constexpr char placeholder = '-';

bool is_opposite(char a, char b)
{
    return (tolower(a) == tolower(b)) &&
           ((islower(a) && isupper(b)) || (isupper(a) && islower(b)));
}

std::string react(std::string str)
{
    for (int ii = 0; ii < str.size(); ++ii)
    {
        if (str[ii] == placeholder) continue;

        int jj = ii + 1;
        while (str[jj] == placeholder) ++jj;

        if (is_opposite(str[ii], str[jj]))
        {
            str[ii] = str[jj] = placeholder;
            while (ii > 0 && str[ii] == placeholder) --ii;
            --ii;
        }
    }

    str.erase(std::remove(std::begin(str), std::end(str), placeholder), std::end(str));
    return str;
}

int main()
{
    std::string input;
    std::getline(std::cin, input);

    int shortest = std::numeric_limits<int>::max();

    for (char ch = 'a'; ch <= 'z'; ++ch)
    {
        std::string str = input;
        std::replace(std::begin(str), std::end(str), (char) tolower(ch), placeholder);
        std::replace(std::begin(str), std::end(str), (char) toupper(ch), placeholder);

        std::string result = react(str);
        if (result.size() < shortest) shortest = result.size();
    }

    std::cout << "length = " << shortest << std::endl;

    return 0;
}