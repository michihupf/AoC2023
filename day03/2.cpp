#include <fstream>
#include <iostream>
#include <ostream>
#include <string>


int find_numbers_on_same_line(int symbol_index, const std::string &s, int &num1, int &num2) {
    int left = std::max(symbol_index - 1, 0);
    int num  = 0;
    int exp  = 1;
    for (;left >= 0 && isdigit(s[left]); left--) {
        short digit  = s[left] - '0';
        num         += exp * digit;
        exp         *= 10;
    }

    bool found_first = false;
    if (num > 0) {
        num1        = num;
        found_first = true;
    }

    num       = 0;
    int right = std::min(symbol_index + 1, (int)s.size());
    for (; right <= (int)s.size() && isdigit(s[right]); right++) {
        short digit  = s[right] - '0';
        num         *= 10;
        num         += digit;
    }

    if (num > 0) {
        if (found_first)
            num2 = num;
        else
            num1 = num;
    }

    return found_first + (num > 0);
}

int find_numbers_on_other_line(int symbol_index, const std::string &s, int &num1, int &num2) {
    if (isdigit(s[symbol_index])) {
        // when we have a number directly at the gear index we can just search to the left
        // until we hit the start and then parse the number
        int idx = symbol_index;
        while (idx > 0 && isdigit(s[--idx]))
            ;
        idx++;

        // idx is now at the start of the number
        int num = 0;
        while (idx <= (int)s.size() && isdigit(s[idx])) {
            short digit  = s[idx++] - '0';
            num         *= 10;
            num         += digit;
        }

        num1 = num;
        return 1;
    }

    // if there is no number at the gear index we might have 2 numbers at max
    //  123.456
    //  ...*...
    int left  = std::max(symbol_index - 1, 0);
    int right = std::min(symbol_index + 1, (int)s.size());

    // search for the left number
    int num = 0;
    if (symbol_index != 0) {
        int exp = 1;
        for (; left >= 0 && isdigit(s[left]); left--) {
            short digit  = s[left] - '0';
            num         += exp * digit;
            exp         *= 10;
        }
    }

    bool found_first = false;
    if (num > 0) {
        num1        = num;
        found_first = true;
    }

    // search for the right number
    num = 0;
    if (symbol_index != (int)s.size() - 1) {
        for (; right < (int)s.size() && isdigit(s[right]); right++) {
            short digit  = s[right] - '0';
            num         *= 10;
            num         += digit;
        }
    }

    if (num > 0) {
        if (found_first)
            num2 = num;
        else
            num1 = num;
    }

    return found_first + (num > 0);
}

int main() {
    std::ifstream infile("./input");

    int    sum = 0;
    std::string line;
    std::string before = "";
    while (std::getline(infile, line)) {
        for (unsigned int i = 0; i < line.size(); i++) {
            // loop until the first *
            char c = line[i];
            if (c != '*')
                continue;

            // we have an *
            // now we check around the possible gear

            // space for at most 6 numbers
            // 123.456
            // 123*456
            // 123.456
            int nums[6]  = {0};
            int count    = find_numbers_on_same_line(i, line, nums[0], nums[1]);
            count       += find_numbers_on_other_line(i, before, nums[2], nums[3]);

            int    pos = infile.tellg();
            std::string after;
            if (std::getline(infile, after)) {
                count += find_numbers_on_other_line(i, after, nums[4], nums[5]);
            }

            infile.seekg(pos, std::ios_base::beg);

            // not a gear
            if (count != 2)
                continue;

            int num1 = 0;
            int num2 = 0;
            for (int j = 0; j < 6; j++) {
                if (nums[j] != 0) {
                    if (num1 == 0)
                        num1 = nums[j];
                    else
                        num2 = nums[j];
                }
            }

            sum += num1 * num2;
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
