#include <fstream>
#include <iostream>
#include <vector>

constexpr bool ASH  = 0;
constexpr bool ROCK = 1;

using ul = unsigned long;

struct Pattern {
    std::vector<ul> rows;
    std::vector<ul> cols;
};


// returns the index of the part before the reflection line
// -1 if no reflection was found
long reflection(const std::vector<ul> &vec) {
    // look for adjacent rows and expand outwards if found
    for (long i = 0; i < (long)vec.size() - 1; i++) {
        bool reflection = true;
        for (long offset = 0; i + 1 + offset < (long)vec.size() && i - offset >= 0;
             offset++) {
            if (vec[i - offset] != vec[i + 1 + offset]) {
                reflection = false;
                break;
            }
        }

        if (reflection)
            return (long)i;
    }

    return -1;
}


int main() {
    std::ifstream infile("./input");

    std::string          line;
    std::vector<Pattern> patterns;
    Pattern              temp;
    std::size_t          row_idx = 0;
    while (std::getline(infile, line)) {
        if (temp.cols.size() == 0)
            temp.cols.resize(line.size());

        if (line == "") {
            patterns.push_back(temp);
            row_idx = 0;
            temp.rows.clear();
            temp.cols = std::vector<ul>();
            continue;
        }

        // we will push bit-masks instead of strings
        ul c = 0;
        for (std::size_t i = 0; i < line.size(); i++) {
            c            |= (line[i] == '#') << (line.size() - i - 1);
            temp.cols[i] |= (line[i] == '#') << row_idx;
        }
        temp.rows.push_back(c);
        row_idx++;
    }
    patterns.push_back(temp);
    infile.close();

    long sum = 0;
    // now we will check adjacent rows and cols for equality
    for (const Pattern &p : patterns) {
        long row_idx = reflection(p.rows);
        if (row_idx != -1) {
            sum += 100 * (row_idx + 1);
            continue;
        }

        long col_idx = reflection(p.cols);
        sum += col_idx + 1;
    }

    infile.close();

    std::cout << "Final number after summarizing notes is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
