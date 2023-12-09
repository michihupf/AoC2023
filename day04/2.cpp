#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>


int main() {
    std::ifstream infile("./input");

    int sum = 0;
    int index = 0;
    std::string line;

    struct identityInt {
        int val = 1;
    };

    std::map<int, identityInt> card_count;
    while (std::getline(infile, line)) {
        std::vector<int> winning{};
        std::vector<int> mine{};

        std::istringstream iss{line};
        // consume the irrelevant characters
        while (iss.peek() != ':')
            iss.get();

        // consume ':' and remaining spaces
        while (!isdigit(iss.peek()))
            iss.get();

        int tmp;
        bool first_half = true;
        while (!iss.eof()) {
            if (isdigit(iss.peek())) {
                iss >> tmp;

                // consume spaces
                while (iss.peek() == ' ')
                    iss.get();

                if (first_half)
                    winning.push_back(tmp);
                else
                    mine.push_back(tmp);

                if (iss.peek() == '|') {
                    // consume '|' and spaces
                    iss.get();
                    while (iss.peek() == ' ')
                        iss.get();

                    first_half = false;
                }
            }
        }

        // by sorting we can just go left to right
        sort(winning.begin(), winning.end());
        sort(mine.begin(), mine.end());

        int max_len = (int) std::min(winning.size(), mine.size());
        
        // depending on the shorter array we need to adjust the condition
        auto inbound = [&winning, &mine, max_len](int i, int j) {
            return winning.size() > mine.size()
                ? j < max_len // mine is smaller
                : i < max_len; // winning is smaller
        };

        int count = 0;
        for (int i = 0, j = 0; inbound(i, j);) {
            if (winning[i] == mine[j]) {
                i++, j++;
                count++; 
            } else if (winning[i] > mine[j]) {
                j++;
            } else {
                i++;
            }
        }

        // we win copies and have one anyway
        sum += card_count[index].val;

        // we now want to add factors for the following cards
        for (int i = 1; i <= count; i++) {
            // every card i wins us one card and we already have 1
            card_count[index + i].val += card_count[index].val;
        }

        index++;
    }
    
    infile.close();

    std::cout << "The sum of card points is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
