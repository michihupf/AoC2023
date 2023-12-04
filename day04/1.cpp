#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    ifstream infile("./input");

    int sum = 0;
    string line;
    while (getline(infile, line)) {
        vector<int> winning{};
        vector<int> mine{};

        istringstream iss{line};
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

        int max_len = (int) min(winning.size(), mine.size());
        
        // depending on the shorter array we need to adjust the condition
        auto inbound = [&winning, &mine, max_len](int i, int j) {
            return winning.size() > mine.size()
                ? j < max_len // mine is smaller
                : i < max_len; // winning is smaller
        };

        int match_pow = 1;
        for (int i = 0, j = 0; inbound(i, j);) {
            if (winning[i] == mine[j]) {
                i++, j++;
                match_pow <<= 1;
            } else if (winning[i] > mine[j]) {
                j++;
            } else {
                i++;
            }
        }

        // shift once to the right as we started with 1 instead of 0
        match_pow >>= 1;
        sum += match_pow;
    }
    
    infile.close();

    cout << "The sum of card points is " << sum << endl;
    cout << "Writing result to ./output" << endl;

    ofstream outfile("./output");
    outfile << sum << endl;
    outfile.close();
}
