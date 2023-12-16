#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Beam {
    // current position
    int x, y;
    // current velocity
    int dx, dy;

    bool operator==(const Beam &other) {
        return other.x == x && other.y == y && other.dx == dx && other.dy == dy;
    }
};

int main() {
    std::ifstream infile("./input");

    std::vector<std::string> layout;

    std::string line;
    while (std::getline(infile, line))
        layout.push_back(line);

    infile.close();

    // beam starts at the top left moving right
    std::deque<Beam> queue;
    queue.push_back({0, 0, 1, 0});
    auto energized = std::vector(layout.size(), std::vector(layout[0].size(), std::vector<Beam>()));
    int  sum       = 0;

    // we will traverse the layout in a bfs style fashion
    // assuming there are no loop we need no extra break condition
    while (!queue.empty()) {
        Beam &beam = queue.front();
        queue.pop_front();

        // see if the beam is still inbounds
        if (beam.x < 0 || beam.x >= (int)layout[0].length())
            continue;
        if (beam.y < 0 || beam.y >= (int)layout[0].length())
            continue;

        if (std::count(energized[beam.x][beam.y].begin(), energized[beam.x][beam.y].end(), beam))
            continue;

        // check if we encounter any mirror or splitter
        char op = layout[beam.y][beam.x];

        // op is a vertical splitter
        if ('.' == op) {
            queue.push_back({beam.x + beam.dx, beam.y + beam.dy, beam.dx, beam.dy});
        } else if ('|' == op) {
            // beam going upwards
            queue.push_back({beam.x, beam.y - 1, 0, -1});
            // beam going downwards
            queue.push_back({beam.x, beam.y + 1, 0, 1});
        } else if ('-' == op) {
            // beam going leftwards
            queue.push_back({beam.x - 1, beam.y, -1, 0});
            // beam going rightwards
            queue.push_back({beam.x + 1, beam.y, 1, 0});
        } else if ('\\' == op) {
            // we need to swap dx and dy
            queue.push_back({beam.x + beam.dy, beam.y + beam.dx, beam.dy, beam.dx});
        } else if ('/' == op) {
            // we need to do dx:=-dy and dy:=-dx
            queue.push_back({beam.x - beam.dy, beam.y - beam.dx, -beam.dy, -beam.dx});
        }

        if (energized[beam.x][beam.y].empty())
            sum++;
        energized[beam.x][beam.y].push_back(beam);
    }

    std::cout << "Number of energized tiles is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
