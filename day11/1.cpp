#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// every Galaxy has x and y coordinates
struct Galaxy {
    int x, y;
};


int main() {
    std::ifstream infile("./input");

    // two vectors to keep track of empty columns
    std::vector<bool> cols;

    std::vector<Galaxy> galaxies;

    std::string line;
    int y = 0;
    while (std::getline(infile, line)) {
        if (cols.empty())
            cols.resize(line.size(), true);

        std::istringstream iss(line);
        char c;
        int x = 0;
        bool row_empty = true;
        while (iss >> c) {
            if (c == '#') {
                galaxies.push_back({x, y});
                row_empty = false;
            }
            cols[x] = cols[x] && c == '.';
            x++;
        }
        // do an extra step for y when the row is empty - effect: doubles the gap
        if (row_empty)
            y++;
        y++;
    }

    // grow the universe in x-direction
    std::vector<int> offset(cols.size(), 0);
    for (int x = 0; x < (int)cols.size(); x++)
        if (cols[x])
            for (int i = x + 1; i < (int)cols.size(); i++)
                offset[i]++;

    for (Galaxy &gal : galaxies)
        gal.x += offset[gal.x];

    int sum = 0;
    // now loop through all possible pairs and 
    // calculate the Manhattan norm
    for (std::size_t i = 0; i < galaxies.size(); i++) {
        for (std::size_t j = i+1; j < galaxies.size(); j++) {
            int dx = std::abs(galaxies[i].x - galaxies[j].x);
            int dy = std::abs(galaxies[i].y - galaxies[j].y);

            sum += dx + dy;
        }
    }
    
    infile.close();

    std::cout << "The sum of the shortest paths between the galaxies is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
