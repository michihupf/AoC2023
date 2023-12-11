#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// for this part we will need some bigger number types

// every Galaxy has x and y coordinates
struct Galaxy {
    long long x, y;
};


int main() {
    std::ifstream infile("./input");

    // two vectors to keep track of empty columns
    std::vector<bool> cols;

    std::vector<Galaxy> galaxies;

    std::string line;
    long long y = 0;
    while (std::getline(infile, line)) {
        if (cols.empty())
            cols.resize(line.size(), true);

        std::istringstream iss(line);
        char c;
        long long x = 0;
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
            y += 1e6 - 1;
        y++;
    }

    // grow the universe in x-direction
    std::vector<long long> offset(cols.size(), 0);
    for (long long x = 0; x < (long long)cols.size(); x++)
        if (cols[x])
            for (long long i = x + 1; i < (long long)cols.size(); i++)
                offset[i] += 1e6 - 1;

    for (Galaxy &gal : galaxies)
        gal.x += offset[gal.x];

    long long sum = 0;
    // now loop through all possible pairs and 
    // calculate the Manhattan norm
    for (std::size_t i = 0; i < galaxies.size(); i++) {
        for (std::size_t j = i+1; j < galaxies.size(); j++) {
            long long dx = std::abs(galaxies[i].x - galaxies[j].x);
            long long dy = std::abs(galaxies[i].y - galaxies[j].y);

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
