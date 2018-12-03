#include <iostream>
#include <string>
#include <map>

int main()
{
    std::map<char, int> letter_counts;

    int cnt_double = 0;
    int cnt_triple = 0;

    while (true)
    {
        std::string current;
        std::cin >> current;
        if (std::cin.eof()) break;

        letter_counts.clear();
        for (char ch : current)
            ++letter_counts[ch];

        bool has_double = false;
        bool has_triple = false;

        for (const auto& entry : letter_counts)
        {
            int count = entry.second;
            if (count == 2) has_double = true;
            if (count == 3) has_triple = true;
        }

        if (has_double) ++cnt_double;
        if (has_triple) ++cnt_triple;
    }

    int result = cnt_double * cnt_triple;
    std::cout << "result = " << result << std::endl;

    return 0;
}