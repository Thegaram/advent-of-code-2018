#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> ids;

    while (true)
    {
        std::string current;
        std::cin >> current;
        if (std::cin.eof()) break;
        ids.push_back(current);
    }

    for (int ii = 0     ; ii < ids.size() - 1; ++ii)
    for (int jj = ii + 1; jj < ids.size()    ; ++jj)
    {
        std::string first  = ids[ii];
        std::string second = ids[jj];

        int cnt_different = 0;
        for (int kk = 0; kk < first.size(); ++kk)
        {
            if (first[kk] != second[kk])
                ++cnt_different;
        }

        if (cnt_different == 1)
        {
            for (int kk = 0; kk < first.size(); ++kk)
                if (first[kk] == second[kk])
                    std::cout << first[kk];
            std::cout << std::endl;
        }
    }

    return 0;
}