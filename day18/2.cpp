#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Instruction {
    long        dx = 0;
    long        dy = 0;
    long        steps;
    std::string color;
};

std::vector<std::complex<long>> create_polygon(const std::vector<Instruction> &instructions) {
    long                            x = 0, y = 0;
    std::vector<std::complex<long>> result;
    for (const Instruction &instruction : instructions) {
        x += instruction.dx * instruction.steps;
        y += instruction.dy * instruction.steps;
        result.emplace_back(x, y);
    }

    return result;
}

// calculates the area of a polygon using the Shoelace formula
// returns <area, number of lattice points on edge>
std::pair<long, long>
calculate_area_and_edge_points(const std::vector<std::complex<long>> &polygon) {
    long area            = 0;
    long num_edge_points = 0;

    const std::complex<long> *last = &polygon.back();
    for (long i = 0; i < (long)polygon.size(); i++) {
        const std::complex<long> *cur = &polygon[i];

        area += (last->real() + cur->real()) * (last->imag() - cur->imag());

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
        iss.ignore(3);
        iss >> tmp.color;
        direction = tmp.color[tmp.color.size() - 2];
        tmp.color = tmp.color.substr(0, 5);
        // you are probably wondering why color is a member
        // of Instruction. I thought it would be more important
        // and now I am too lazy to fix it.

        // now that we know someone gave us the wrong data we
        // will ignore direction and steps values we are given
        // and infer the values from the color

        tmp.steps = std::stoul(tmp.color, nullptr, 16);

        switch (direction) {
        case '0': // R
            tmp.dx = 1;
            tmp.dy = 0;
            break;
        case '3': // U
            tmp.dx = 0;
            tmp.dy = -1;
            break;
        case '2': // L
            tmp.dx = -1;
            tmp.dy = 0;
            break;
        case '1': // D
            tmp.dx = 0;
            tmp.dy = 1;
            break;
        default:
            std::cout << direction;
            exit(1);
        }

        instructions.push_back(tmp);
    }

    infile.close();

    std::vector<std::complex<long>> polygon = create_polygon(instructions);

    auto [A, R]                            = calculate_area_and_edge_points(polygon);

    // according to Pick's Theorem the number of interior points will be equal to
    // A - R/2 + 1

    long I   = A - R / 2 + 1;
    long sum = R + I;

    std::cout << "Amount of lava that can be contained is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
