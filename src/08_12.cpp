#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <vector>

class Tree
{
private:
    static int next_id;

    struct Node
    {
        std::vector<int> children;
        std::vector<int> meta;
    };

    std::map<int, Node> tree;

public:
    int read(std::istream& is)
    {
        int id = next_id++;
        Node& node = tree[id];

        int cnt_children, cnt_meta;
        is >> cnt_children >> cnt_meta;

        for (int ii = 0; ii < cnt_children; ++ii)
        {
            int child = read(is);
            node.children.push_back(child);
        }

        for (int ii = 0; ii < cnt_meta; ++ii)
        {
            int meta; is >> meta;
            node.meta.push_back(meta);
        }

        return id;
    }

    std::vector<int> children_of(int id) const
    {
        return tree.at(id).children;
    }

    std::vector<int> meta_of(int id) const
    {
        return tree.at(id).meta;
    }
};

int Tree::next_id = 0;

int sum_meta(const Tree& tree, int root)
{
    int sum = 0;
    for (int m : tree.meta_of(root))     sum += m;
    for (int c : tree.children_of(root)) sum += sum_meta(tree, c);
    return sum;
}

int value(const Tree& tree, int root)
{
    const auto& meta = tree.meta_of(root);
    const auto& children = tree.children_of(root);

    if (children.empty())
        return std::accumulate(std::begin(meta), std::end(meta), 0);

    return std::accumulate(std::begin(meta), std::end(meta), 0, [&tree, &children](int acc, int m) {
        int id = m - 1;
        return (id < 0 || id >= children.size()) ? acc : acc + value(tree, children[id]);
    });
}

int main()
{
    Tree t;
    int root = t.read(std::cin);

    std::cout << sum_meta(t, root) << std::endl;
    std::cout << value(t, root) << std::endl;

    return 0;
}