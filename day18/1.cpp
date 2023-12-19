#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Instruction {
    int         dx = 0;
    int         dy = 0;
    int         steps;
    std::string color;
};

std::vector<std::complex<int>> create_polygon(const std::vector<Instruction> &instructions) {
    int                            x = 0, y = 0;
    std::vector<std::complex<int>> result;
    for (const Instruction &instruction : instructions) {
        x += instruction.dx * instruction.steps;
        y += instruction.dy * instruction.steps;
        result.emplace_back(x, y);
    }

    return result;
}

// calculates the area of a polygon using the Shoelace formula
// returns <area, number of lattice points on edge>
std::pair<int, int> calculate_area_and_edge_points(const std::vector<std::complex<int>> &polygon) {
    int area            = 0;
    int num_edge_points = 0;

    const std::complex<int> *last = &polygon.back();
    for (int i = 0; i < (int)polygon.size(); i++) {
        const std::complex<int> *cur  = &polygon[i];
        area                         += (last->real() + cur->real()) * (last->imag() - cur->imag());

        num_edge_points += std::abs(cur->real() - last->real()); // x diff
        num_edge_points += std::abs(cur->imag() - last->imag()); // y diff

        last = cur;
    }

    return std::make_pair(std::abs(area / 2), num_edge_points);
}

int main() {
    std::ifstream infile("./input");

    std::vector<Instruction> instructions;
    std::string              line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        Instruction tmp;
        char        direction;
        iss >> direction >> tmp.steps;
        iss.ignore(2);
        iss >> tmp.color;
        tmp.color = tmp.color.substr(0, 7);

        switch (direction) {
        case 'R':
            tmp.dx = 1;
            tmp.dy = 0;
            break;
        case 'U':
            tmp.dx = 0;
            tmp.dy = -1;
            break;
        case 'L':
            tmp.dx = -1;
            tmp.dy = 0;
            break;
        case 'D':
            tmp.dx = 0;
            tmp.dy = 1;
            break;
        default:
            exit(1);
        }

        instructions.push_back(tmp);
    }

    infile.close();

    std::vector<std::complex<int>> polygon = create_polygon(instructions);
    auto [A, R]                            = calculate_area_and_edge_points(polygon);

    // according to Pick's Theorem the number of interior points will be equal to
    // A - R/2 + 1

    int I   = A - R / 2 + 1;
    int sum = R + I;

    std::cout << "Amount of lava that can be contained is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
