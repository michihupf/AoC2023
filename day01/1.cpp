#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main() {
    unsigned int sum = 0;
    string       line;

    // for every line calculate the calibration code and sum up
    ifstream infile("./input");
    while (getline(infile, line)) {
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
    cout << "Sum of Calibration Codes is: " << sum << endl;
    cout << "Writing result to ./output" << endl;

    ofstream outfile("./output");
    outfile << sum << endl;
    outfile.close();
}
