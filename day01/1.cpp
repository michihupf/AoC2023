#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    unsigned int sum = 0;
    std::string       line;

    // for every line calculate the calibration code and sum up
    std::ifstream infile("./input");
    while (std::getline(infile, line)) {
        // search first digit
        for (int i = 0; i < (int)line.size(); i++) {
            if (std::isdigit(line[i])) {
                sum += 10 * (line[i] - '0');
                break;
            }
        }

        // search for last digit
        for (int i = line.size() - 1; i >= 0; i--) {
            if (std::isdigit(line[i])) {
                sum += line[i] - '0';
                break;
            }
        }
    }
    infile.close();

    // print result
    std::cout << "Sum of Calibration Codes is: " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
