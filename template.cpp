#include <fstream>
#include <iostream>


int main() {
    std::ifstream infile("./input");

    int sum = 0;
    // CODE GOES HERE
    
    infile.close();

    std::cout << "MESSAGE" << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
