#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

inline bool is_symbol(char c) {
    return c != '.' && c != '\n' && !isdigit(c);
}

int main() {
    std::ifstream infile("./input");

    int    sum = 0;
    std::string line;
    std::string before = "";
    while (std::getline(infile, line)) {
        int num   = 0;
        int start = 0;
        for (unsigned int i = 0; i < line.size(); i++) {
            // loop until the first number
            char c = line[i];
            if (!isdigit(c) && num == 0)
                continue;

            // we have a number here
            if (isdigit(c)) {
                unsigned int digit = c - '0';

                // set the beginning index of the number
                if (num == 0)
                    start = i;

                num *= 10;
                num += digit;

                // when we are the the end we need to check NOW
                if (i + 1 < line.size())
                    continue;
            }


            // check if symbol is horizonally adjacent
            int left  = start - 1;
            int right = i;

            if (left >= 0 && is_symbol(line[left])) {
                sum += num;
                num = 0;
                continue;
            }

            if (is_symbol(line[right])) {
                sum += num;
                num = 0;
                continue;
            }

            right++; // move right behind the character for following loop conditions

            // check for vertical and diagonal smybol
            // check the line above
            bool done = false;
            for (int j = std::max(left, 0); j < std::min(right, (int)before.size()); j++) {
                if (is_symbol(before[j])) {
                    sum  += num;
                    num   = 0;
                    done  = true;
                    break;
                }
            }

            if (done)
                continue;

            // check the line after
            int pos = infile.tellg();

            std::string after;

            if (!std::getline(infile, after)) {
                num = 0;
                continue;
            }

            for (int j = std::max(left, 0); j < std::min(right, (int)after.size()); j++) {
                if (is_symbol(after[j])) {
                    sum += num;
                    done = true;
                    break;
                }
            }

            infile.seekg(pos, std::ios_base::beg);

            // if nothing did anything reset num
            num = 0;
        }

        before = std::move(line);
    }

    infile.close();

    std::cout << "Sum of engine part numbers is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
