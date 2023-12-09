#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


// returns the predicted value of nums
long predict_step(const std::vector<long> &nums) {
    // check if all zeors
    bool non_zero = false;
    for (const long l : nums)
        if (l != 0) {
            non_zero = true;
            break;
        }

    if (!non_zero)
        return 0;

    // calculate difference vector
    std::vector<long> diff;
    for (std::size_t i = 0; i < nums.size() - 1; i++)
        diff.push_back(nums[i + 1] - nums[i]);

    // now recurse on predict using the difference
    return diff.back() + predict_step(diff);
}

int main() {
    std::ifstream infile("./input");

    // for every row of the input make a prediction
    int sum = 0;
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<long> nums;
        long v;
        while (iss >> v)
            nums.push_back(v);

        sum += nums.back() + predict_step(nums);
    }

    infile.close();

    std::cout << "The sum of the extraolated values is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
