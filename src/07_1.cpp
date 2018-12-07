#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <string>

class DAG
{
private:
    struct Node
    {
        std::set<char> predecessors;
        std::set<char> successors;
    };

    std::map<char, Node> dag;

public:
    void addNode(char id)
    {
        if (dag.count(id) == 0)
            dag[id] = Node();
    }

    void addEdge(char from, char to)
    {
        addNode(from);
        addNode(to);

        dag[from].successors.insert(to);
        dag[to].predecessors.insert(from);
    }

    void removeNode(char id)
    {
        for (char pred : dag[id].predecessors)
            dag[pred].successors.erase(id);

        for (char succ : dag[id].successors)
            dag[succ].predecessors.erase(id);

        dag.erase(id);
    }

    std::optional<char> executeNext()
    {
        // map is ordered so we are guaranteed to find the smallest one
        auto it = std::find_if(std::begin(dag), std::end(dag), [](const auto& entry) {
            return entry.second.predecessors.empty();
        });

        if (it == std::end(dag))
            return {};

        char id = it->first;
        removeNode(id);
        return id;
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

    while (true)
    {
        auto next = dag.executeNext();
        if (!next) break;
        std::cout << *next;
    }

    std::cout << std::endl;

    return 0;
}