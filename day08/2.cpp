#include <numeric>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

struct Node {
    Node       *left;
    Node       *right;
    std::string name;
    bool        is_end;
};

struct Graph {
    std::map<std::string, Node> nodes;
    std::vector<Node *>         starting_nodes;

    Node *get_node(std::string name) {
        if (0 == nodes.count(name))
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
            Node *existing  = get_node(name);
            existing->left  = l;
            existing->right = r;

            return existing;
        } else {
            nodes[name] = Node{l, r, name, 'Z' == name[2]};
            Node *nn    = &nodes[name];

            if ('A' == name[2])
                starting_nodes.push_back(nn);

            return nn;
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

        if (0 == node[0].size())
            break;

        // remove '(', ',' and ')'
        node[1] = node[1].substr(1, 3);
        node[2] = node[2].substr(0, 3);

        g.add_node(node[0], node[1], node[2]);
    }

    // g is a graph that contains the entire network structure
    // moving accoring to rli will result in the answer

    // actually doing it simultaneous will probably take a
    // while as my PC is not a ghost (and not a quantum
    // computer either). Anyway we will look for cycles
    // such that the current node we are looking at is an
    // end node and it points to an element that was pointed
    // to by the start node and also matches the rli cycle meaning
    // rli_idx = 0 when encountering the end node.
    //
    // This works because the end nodes and start nodes point to the
    // same nodes. I checked using grep(1).
    //
    // The total amount of steps required would then be
    // the LCM of every singular cycle length.

    long long              steps   = 1;
    int                    rli_idx = 0;
    std::vector<long long> node_steps;
    for (Node *current : g.starting_nodes) {
        // save those for later
        Node *l = current->left;
        Node *r = current->right;

        long long local_steps = 0;
        while (true) {
            if (rli[rli_idx] == 'R')
                current = current->right;
            else
                current = current->left;

            local_steps++;

            rli_idx++;
            rli_idx %= rli.size();

            if (current->is_end && 0 == rli_idx && (current->left == l || current->left == r) &&
                (current->right == l || current->right == r)) {
                break;
            }
        }

        node_steps.push_back(local_steps);
    }

    for (const long long s : node_steps) {
        steps = std::lcm(steps, s);
    }

    infile.close();

    std::cout << "The number of steps required is " << steps << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << steps << std::endl;
    outfile.close();
}
