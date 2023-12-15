#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int hash(const std::string &str) {
    int h = 0;
    for (const char c : str) {
        h += c;
        h *= 17;
        h %= 256;
    }

    return h;
}

int main() {
    std::ifstream infile("./input");

    // read the input
    std::vector<std::string> steps;
    std::string line;
    while (std::getline(infile, line, ','))
        steps.push_back(line);

    // last line contains \n
    steps.back() = steps.back().erase(steps.back().size() - 1, 1);

    int sum = 0;
    // compute the HASH for every step and add up
    for (const std::string &step : steps) {
        sum += hash(step);
    }
    
    infile.close();

    std::cout << "Sum of hashes is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
