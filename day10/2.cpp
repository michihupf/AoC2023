#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>

struct Pipe {
    int         x, y;
    char                type;
    std::vector<Pipe *> neighbors;
    // whether or not we hit the pipe in search
    bool hit = false;
};

using Schematic = std::vector<std::vector<Pipe>>;

const std::array<char, 4> DIRECTIONS = {'l', 'r', 'u', 'd'};
std::map<char, std::vector<char>> directions_of = {
    { '-', {'l', 'r'}},
    { '|', {'u', 'd'}},
    { 'S', std::vector<char>(DIRECTIONS.begin(), DIRECTIONS.end())},
    { '7', {'l', 'd'}},
    { 'F', {'d', 'r'}},
    { 'J', {'l', 'u'}},
    { 'L', {'u', 'r'}},
};

bool connected_to(char direction, int x, int y, const Schematic &schematic) {
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
        if (x >= (int)schematic[0].size() - 1)
            return false;
        return check_all(schematic[y][x + 1], {'-', '7', 'J', 'S'});
    case 'u':
        if (y <= 0)
            return false;
        return check_all(schematic[y - 1][x], {'|', '7', 'F', 'S'});
    case 'd':
        if (y >= (int)schematic.size() - 1)
            return false;
        return check_all(schematic[y + 1][x], {'|', 'J', 'L', 'S'});
    }
    return false;
}

Pipe *get_neighbor(char direction, int x, int y, Schematic &schematic) {
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

// performs DFS on a graph with starting node start and
// fills poly_lines vector with polygon edges
void dfs(Schematic &schematic, Pipe *start, std::vector<std::pair<Pipe *, Pipe *>> &poly_lines) {
    std::vector<Pipe *> stack;
    stack.push_back(start);

    Pipe *first_corner = nullptr;
    while (!stack.empty()) {
        Pipe *node = stack.back();
        stack.pop_back();

        if (node->hit) {
            continue;
        }

        node->hit = true;
        for (Pipe *child : node->neighbors) {
            stack.push_back(child);
        }

        // we will ignore non-corner nodes
        if (node->type == '.' || node->type == '|' || node->type == '-')
            continue;

        if (first_corner) {
            poly_lines.push_back(std::make_pair(first_corner, node));
        }
        
        first_corner = node;
    }
    poly_lines.push_back(std::make_pair(first_corner, start));
}

int main() {
    std::ifstream infile("./input");

    // read the schematic and parse pipes
    char        c;
    int x = 0, y = 0;
    Pipe       *start = nullptr;
    Schematic   schematic;
    schematic.push_back(std::vector<Pipe>());
    while (infile.get(c)) {
        if ('\n' == c) {
            schematic.push_back(std::vector<Pipe>());
            y++;
            x = 0;
            continue;
        }
        schematic[y].push_back({x, y, c});
        x++;
    }

    // remove extra line at end
    schematic.erase(schematic.rbegin().base());

    for (int y = 0; y < (int)schematic.size(); y++) {
        for (int x = 0; x < (int)schematic[0].size(); x++) {
            Pipe *pipe = &schematic[y][x];

            // skip ground
            if ('.' == pipe->type)
                continue;

            // set neighbors
            for (char direction : directions_of[pipe->type]) {
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
    // we will do a DFS to walk along the loop

    // to find the enclosed area we will accumulate all corner pipes
    // 7, F, J, L and S. Those corner pipes form a polygon which 
    // encloses an area.

    std::vector<std::pair<Pipe *, Pipe *>> lines;
    dfs(schematic, start, lines);

    // every pipe has an x and y coordinate. We can check whether a 
    // point is inside or outside of a polygon by using the 
    // ray casting algorithm. The idea is to cast a ray from the 
    // point into any direction.
    
    // There are still problems with the case when a point is directly 
    // under a vertical line. Inside and outside is ambigous when under 
    // a vertical line. We will shift every lattice point by 0.5 to the right 
    // and we avoid this issue entirely :D
    
    int inside_count = 0;
    for (int y = 0; y < (int)schematic.size(); y++) {
        for (int x = 0; x < (int)schematic[0].size(); x++) {

            // now do an intersection test with every edge of our polygon
            // we need to handle the edge case where a tile is directly below a 
            // vertical pipe segment
            int intersection_count = 0;
            for (const auto &[c1, c2] : lines) {
                int x_min = std::min(c1->x, c2->x);
                int x_max = std::max(c1->x, c2->x);
                int y_min = std::min(c1->y, c2->y);

                // when the tile is part of the line we need to skip computation
                if (schematic[y][x].hit) {
                    intersection_count = 0;
                    break;
                }

                // skip vertical line
                if (x_max == x_min)
                    continue;

                if (y > y_min && x + 0.5 < x_max && x + 0.5 > x_min) {
                    intersection_count++;
                }
            }

            // an odd intersection count means inside
            inside_count += intersection_count % 2;
        }
    }

    infile.close();

    std::cout << "The amount of points inside is " << inside_count << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << inside_count << std::endl;
    outfile.close();
}
