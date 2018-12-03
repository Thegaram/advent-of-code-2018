#include <iostream>
#include <vector>
#include <set>

int main()
{
    std::vector<int> change_list;

    while (true)
    {
        int current;
        std::cin >> current;
        if (std::cin.eof()) break;
        change_list.push_back(current);
    }

    int frequency = 0;
    std::set<int> frequencies_seen;
    frequencies_seen.insert(0);

    bool found = false;
    while (!found)
    {
        for (int val : change_list)
        {
            frequency += val;

            if (frequencies_seen.count(frequency) > 0)
            {
                found = true;
                break;
            }

            frequencies_seen.insert(frequency);
        }
    }

    std::cout << "result = " << frequency << std::endl;

    return 0;
}