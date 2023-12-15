#include <fstream>
#include <iostream>
#include <array>
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

struct Step {
    std::string label;
    char operation;
    int focal_length = -1; // unused with operation = -
};

using Box = std::vector<Step>;
using BoxMap = std::array<Box, 256>;

int main() {
    std::ifstream infile("./input");

    // read the input
    std::vector<Step> steps;
    std::string line;
    while (std::getline(infile, line, ',')) {
        Step step;
        bool label_done = false;
        for (const char c : line) {
            if ('-' == c || '=' == c) {
                step.operation = c;
                label_done = true;
            } else if (label_done) {
                if ('\n' == c)
                    continue;

                step.focal_length = c - '0';
            } else {
                step.label.push_back(c);
            }
        }
        steps.push_back(step);
    }

    infile.close();

    // map steps to boxes
    BoxMap boxes;
    for (const Step &step : steps) {
        int h = hash(step.label);
        Box &box = boxes[h];

        // index of step in box
        int idx = -1;
        for (int i = 0; i < (int)box.size(); i++) {
            if (step.label == box[i].label) {
                idx = i;
                break;
            }
        }

        if (idx >= 0) {
            if ('-' == step.operation) {
                box.erase(box.begin() + idx);
            } else {
                box[idx].focal_length = step.focal_length;
            }
        } else if ('=' == step.operation)
            box.push_back(step);
    }


    int sum = 0;
    // compute the HASH for every step and add up
    for (int i = 0; i < (int)boxes.size(); i++) {
        Box &box = boxes[i];
        for (int slot = 0; slot < (int)box.size(); slot++) {
            sum += (i + 1) * (slot + 1) * box[slot].focal_length;
        }
    }
    
    std::cout << "Sum of hashes is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
