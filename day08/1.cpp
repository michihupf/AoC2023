#include <array>
#include <fstream>
#include <iostream>
#include <map>

struct Node {
    Node       *left;
    Node       *right;
    std::string name;
};

struct Graph {
    std::map<std::string, Node> nodes;

    Node *get_node(std::string name) {
        if (nodes.count(name) == 0)
            return nullptr;

        return &nodes[name];
    }

    Node *add_node(std::string name, std::string left, std::string right) {
        // node already in
        Node *l = get_node(left);
        Node *r = get_node(right);

        if (!l && left != "---")
            l = add_node(left, "---", "---");
        if (!r && right != "---")
            r = add_node(right, "---", "---");

        if (nodes.count(name)) {
            Node *existing = get_node(name);
            existing->left = l;
            existing->right = r;

            return existing;
        } else {
            nodes[name] = Node{l, r, name};
            
            return &nodes[name];
        }
    }
};

int main() {
    std::ifstream infile("./input");

    // read the input
    std::string rli;
    std::getline(infile, rli); // first line is RL instruction
    infile.ignore(1000, '\n');
    
    using NodeStr = std::array<std::string, 3>;

    Graph g;
    while (!infile.eof()) {
        NodeStr node;
        std::getline(infile, node[0], ' ');
        // this catches '='
        std::getline(infile, node[1], ' ');
        std::getline(infile, node[1], ' ');
        std::getline(infile, node[2], '\n');

        if (node[0].size() == 0)
            break;

        // remove '(', ',' and ')'
        node[1] = node[1].substr(1, 3);
        node[2] = node[2].substr(0, 3);

        g.add_node(node[0], node[1], node[2]);
    }

    // g is a graph that contains the entire network structure
    // moving accoring to rli will result in the answer

    int   steps   = 0;
    int   rli_idx = 0;
    Node *current = g.get_node("AAA");
    while (current->name != "ZZZ") {
        if (rli[rli_idx] == 'R') {
            current = current->right;
        } else {
            current = current->left;
        }
        rli_idx++;
        steps++;
        rli_idx %= rli.length();
    }

    infile.close();

    std::cout << "The number of steps required is " << steps << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << steps << std::endl;
    outfile.close();
}
