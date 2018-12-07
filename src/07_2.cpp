#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>

constexpr int BASETIME = 60;
constexpr int MAXWORKERS = 5;

class DAG
{
private:
    struct Node
    {
        std::set<char> predecessors;
        std::set<char> successors;
    };

    std::map<char, Node> dag;
    std::map<char, int> inProgress;

public:
    void addNode(char id)
    {
        if (dag.count(id) == 0)
            dag[id] = Node();
    }

    void addEdge(char before, char after)
    {
        addNode(before);
        addNode(after);

        dag[before].successors.insert(after);
        dag[after].predecessors.insert(before);
    }

    void removeNode(char id)
    {
        for (char pred : dag[id].predecessors)
            dag[pred].successors.erase(id);

        for (char succ : dag[id].successors)
            dag[succ].predecessors.erase(id);

        dag.erase(id);
    }

    bool stepTime()
    {
        // step time & mark completed tasks
        std::set<char> completedJustNow;
        for (auto& entry : inProgress)
        {
            char id = entry.first;
            int& timeLeft = entry.second;

            if (--timeLeft == 0)
                completedJustNow.insert(id);
        }

        // remove completed tasks
        for (char id : completedJustNow)
        {
            inProgress.erase(id);
            removeNode(id);
        }

        // add newly available tasks
        for (const auto& entry : dag)
        {
            char id = entry.first;
            if ((inProgress.count(id) == 0) && dag[id].predecessors.empty() && (inProgress.size() < MAXWORKERS))
                inProgress[id] = BASETIME + (id - 'A' + 1);
        }

        return dag.size() == 0;
    }
};

int main()
{
    std::regex regex {R"(^Step (\w) must be finished before step (\w) can begin.$)"};

    DAG dag;

    while (true)
    {
        std::string current;
        std::getline(std::cin, current);
        if (std::cin.eof()) break;

        std::smatch matches;
        if (std::regex_search(current, matches, regex))
        {
            char before = matches[1].str()[0];
            char after  = matches[2].str()[0];
            dag.addEdge(before, after);
        }
    }

    int count = 0;
    while (true)
    {
        bool completed = dag.stepTime();
        if (completed) break;
        ++count;
    }

    std::cout << count << std::endl;

    return 0;
}