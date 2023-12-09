#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

// reads a number discarding whitespace
long long make_big_number(std::istringstream &iss) {
    long long current, x = 0;
    while (iss >> current) {
        int len = std::to_string(current).length();
        x *= std::pow(10, len);
        x += current;
    }

    return x;
}

int main() {
    std::ifstream infile("./input");

    long long prod = 1;
    std::string times, dists;
    std::getline(infile, times);
    std::getline(infile, dists);

    infile.close();

    std::istringstream times_ss{times};
    std::istringstream dists_ss{dists};

    // "Time:" is 5 chars, "Distance:" is 9
    times_ss.ignore(5);
    dists_ss.ignore(9);

    long long T = make_big_number(times_ss);
    long long D = make_big_number(dists_ss);

    // we will solve the problem mathematically
    //
    // Let tM be the time spent moving, tS the time spent stationary
    // and T the race time. We know that the distance traveled 
    // d = v * tM but we know v = tS and we also know tM = T - tS
    // so it follows that d = tS (T - tS).
    //
    // Rearranging gives 
    // 0 = -tS^2 + TtS - d = p(tS)
    //
    // The zeros of the parabola describe the points where a 
    // distance of d. is reached. Every p(tS) > 0 we know we win the race, so we 
    // just need to count every integer between tS1 and tS2. We use the p-q-formula
    // to obtain the solutions:
    
    // the tS are upper and lower bound
    long double upper_bound = T / 2.0 + std::sqrt(T * T / 4.0 - D);
    long double lower_bound = T / 2.0 - std::sqrt(T * T / 4.0 - D);

    // inclusive integer bounds
    long long lb = std::floor(lower_bound) + 1;
    long long ub = std::ceil(upper_bound) - 1;

    // multiply prod with amount of integer solutions to p(tS) > 0
    prod *= ub - lb + 1;

    
    infile.close();

    std::cout << "Multiplying the numbers together yields " << prod << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << prod << std::endl;
    outfile.close();
}
