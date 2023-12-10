#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

struct Pipe {
    std::size_t         x, y;
    char                type;
    std::vector<Pipe *> neighbors;
    // whether or not we hit the pipe in search
    bool hit = false;
};

using Schematic = std::vector<std::vector<Pipe>>;

const std::array<char, 4> DIRECTIONS = {'l', 'r', 'u', 'd'};

bool connected_to(char direction, std::size_t x, std::size_t y, const Schematic &schematic) {
    auto check_all = [](const Pipe &node, std::initializer_list<char> candidates) {
        return std::any_of(candidates.begin(), candidates.end(),
                           [&node](char c) { return c == node.type; });
    };

    switch (direction) {
    case 'l':
        if (x <= 0)
            return false;
        return check_all(schematic[y][x - 1], {'-', 'F', 'L', 'S'});
    case 'r':
        if (x >= schematic[0].size() - 1)
            return false;
        return check_all(schematic[y][x + 1], {'-', '7', 'J', 'S'});
    case 'u':
        if (y <= 0)
            return false;
        return check_all(schematic[y - 1][x], {'|', '7', 'F', 'S'});
    case 'd':
        if (y >= schematic.size() - 1)
            return false;
        return check_all(schematic[y + 1][x], {'|', 'J', 'L', 'S'});
    }
    return false;
}

Pipe *get_neighbor(char direction, std::size_t x, std::size_t y, Schematic &schematic) {
    if (!connected_to(direction, x, y, schematic))
        return nullptr;

    switch (direction) {
    case 'l':
        return &schematic[y][x - 1];
    case 'r':
        return &schematic[y][x + 1];
    case 'u':
        return &schematic[y - 1][x];
    case 'd':
        return &schematic[y + 1][x];
    }

    return nullptr;
}

// performs BFS on a graph with starting node start and
// returns reached depth
int bfs(Schematic &schematic, Pipe *start) {
    std::deque<Pipe *> q;
    q.push_back(start);
    // we will have a nullptr as a border between levels
    q.push_back(nullptr);
    start->hit = true;

    int depth = 0;
    while (!q.empty()) {
        Pipe *node = q.front();
        q.pop_front();

        if (node == nullptr) {
            q.push_back(nullptr);
            if (q.front() == nullptr) // we are done
                break;

            depth++;
            continue;
        }

        for (Pipe *child : node->neighbors) {
            if (child->hit)
                continue;

            q.push_back(child);
            child->hit = true;
        }
    }

    // as we know that we have a closed path
    return depth;
}

int main() {
    std::ifstream infile("./input");

    // read the schematic and parse pipes
    char        c;
    std::size_t x = 0, y = 0;
    Pipe       *start = nullptr;
    Schematic   schematic;
    schematic.push_back(std::vector<Pipe>());
    while (infile.get(c)) {
        if ('\n' == c) {
            schematic.push_back(std::vector<Pipe>());
            y++;
            continue;
        }
        schematic[y].push_back({x, y, c});
        x++;
    }

    // remove extra line at end
    schematic.erase(schematic.rbegin().base());

    for (std::size_t y = 0; y < schematic.size(); y++) {
        for (std::size_t x = 0; x < schematic[0].size(); x++) {
            Pipe *pipe = &schematic[y][x];

            // skip ground
            if ('.' == pipe->type)
                continue;

            // set neighbors
            for (char direction : DIRECTIONS) {
                Pipe *neigh = get_neighbor(direction, x, y, schematic);

                if (neigh) {
                    pipe->neighbors.push_back(neigh);
                }
            }

            if ('S' == pipe->type)
                start = pipe;
        }
    }

    // now that we have our graph we can traverse it
    // we will do a BFS from start to find the longest path
    // (assumption: the loop is the longest connected path)

    // the graph will allow a full BFS with pretty fast runtime
    // as we have few paths to travel along

    int steps = bfs(schematic, start);

    infile.close();

    std::cout << "The amount of steps needed to reach the farthest point is " << steps << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << steps << std::endl;
    outfile.close();
}
