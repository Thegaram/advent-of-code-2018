#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <regex>
#include <set>
#include <string>

struct Date
{
    std::string str;
    int year;
    int month;
    int day;
    int hour;
    int minute;
};

enum class EntryType { Shift, Wakes, Sleeps };

struct Entry
{
    Date date;
    EntryType type;
    int guard; // id
};

void read_entries(std::istream& in, std::vector<Entry>& entries, std::set<int>& guards)
{
    std::regex regex {R"(^(\[(\d+)-(\d+)-(\d+) (\d+):(\d+)\]) (?:(Guard) #(\d+) begins shift|(wakes) up|(falls) asleep)$)"};

    while (true)
    {
        std::string current;
        std::getline(std::cin, current);
        if (std::cin.eof()) break;

        std::smatch matches;

        if (!std::regex_search(current, matches, regex))
            continue;

        std::string date_str = matches[1];
        int year   = std::stoi(matches[2]);
        int month  = std::stoi(matches[3]);
        int day    = std::stoi(matches[4]);
        int hour   = std::stoi(matches[5]);
        int min = std::stoi(matches[6]);

        Date date = {date_str, year, month, day, hour, min};

        EntryType type = (matches[7].length() > 0) ? EntryType::Shift :
                         (matches[9].length() > 0) ? EntryType::Wakes :
                                                     EntryType::Sleeps;

        int guard = (matches[8].length() > 0) ? std::stoi(matches[8]) : -1;
        if (guard != -1) guards.insert(guard);

        entries.push_back({date, type, guard});
    }
}

void normalize_entries(std::vector<Entry>& entries)
{
    std::sort(std::begin(entries), std::end(entries), [](const Entry& a, const Entry& b) {
        return a.date.str < b.date.str;
    });

    int current_guard = -1;
    for (auto& entry : entries)
    {
        if (entry.type == EntryType::Shift)
        {
            current_guard = entry.guard;
        }
        else
        {
            assert (current_guard != -1);
            entry.guard = current_guard;
        }
    }
}

int count_mins_asleep(const std::vector<Entry>& entries, int guard, std::array<int, 60>& min_cnts)
{
    int mins = 0;
    int start = -1;

    for (auto entry : entries)
    {
        if (entry.guard != guard) continue;

        if (entry.type == EntryType::Sleeps)
        {
            assert (start == -1);
            start = entry.date.minute;
        }
        else if (entry.type == EntryType::Wakes)
        {
            assert (start != -1);

            int end = entry.date.minute;
            mins += end - start;

            for (int minute = start; minute < end; ++minute)
                ++min_cnts[minute];

            start = -1;
        }
    }

    return mins;
}

int main()
{
    std::vector<Entry> entries;
    std::set<int> guards;

    read_entries(std::cin, entries, guards);
    normalize_entries(entries);

    int sleepiest_guard = -1;
    int which_min = -1;
    int how_many_times = -1;

    for (int guard : guards)
    {
        std::array<int, 60> min_cnts = {};
        count_mins_asleep(entries, guard, min_cnts);

        int max_min_cnt = -1;
        int max_min = -1;

        for (int min = 0; min < 60; ++min)
        {
            if (min_cnts[min] > max_min_cnt)
            {
                max_min_cnt = min_cnts[min];
                max_min = min;
            }
        }

        if (max_min_cnt > how_many_times)
        {
            sleepiest_guard = guard;
            how_many_times  = max_min_cnt;
            which_min       = max_min;
        }
    }

    int result = sleepiest_guard * which_min;
    std::cout << "sleepiest guard: #" << sleepiest_guard << std::endl;
    std::cout << "sleep at min: "     << which_min       << std::endl;
    std::cout << "result: "           << result          << std::endl;

    return 0;
}