#include <iostream>

int main()
{
    int frequency = 0;
    while (true)
    {
        int current;
        std::cin >> current;
        if (std::cin.eof()) break;
        frequency += current;
    }

    std::cout << "result = " << frequency << std::endl;

    return 0;
}