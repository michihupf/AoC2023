#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>

enum class HandType : long {
    HIGH_CARD  = 1,
    ONE_PAIR   = 2,
    TWO_PAIR   = 3,
    THREE_KIND = 4,
    FULL_HOUSE = 5,
    FOUR_KIND  = 6,
    FIVE_KIND  = 7
};

struct Hand {
    std::string repr;
    int         bid;
    HandType    type;
    long        sort_key;
};

const std::map<char, long> value_of = {
    {'2', 0 },
    {'3', 1 },
    {'4', 2 },
    {'5', 3 },
    {'6', 4 },
    {'7', 5 },
    {'8', 6 },
    {'9', 7 },
    {'T', 8 },
    {'J', 9 },
    {'Q', 10},
    {'K', 11},
    {'A', 12}
};

int main() {
    std::ifstream infile("./input");

    // read in hands
    std::vector<Hand> hands;
    std::string       line;
    while (std::getline(infile, line)) {
        // <HAND> <BID>
        std::string        hand_str;
        std::istringstream iss{line};
        std::getline(iss, hand_str, ' ');

        int bid;
        iss >> bid;
        hands.push_back({hand_str, bid});
    }

    struct int_0_t {
        int val = 0;
    };

    // will store the ranking where ranking[i] is of rank i+1
    std::vector<Hand *> ranking;

    // what's the type of hand we got
    for (auto &hand : hands) {
        std::istringstream iss{hand.repr};

        std::map<char, int_0_t> token_count;
        char                    c;
        while (iss >> c)
            token_count[c].val++;

        int max = 0;
        for (const auto &[tok, count] : token_count) {
            if (count.val > max)
                max = count.val;
        }

        switch (token_count.size()) {
        case 1:
            // five of a kind
            hand.type = HandType::FIVE_KIND;
            break;
        case 2:
            // four of a kind or full house
            if (max == 4)
                hand.type = HandType::FOUR_KIND;
            else
                hand.type = HandType::FULL_HOUSE;
            break;
        case 3:
            // either three of a kind or two pair
            if (max == 3)
                hand.type = HandType::THREE_KIND;
            else
                hand.type = HandType::TWO_PAIR;
            break;
        case 4:
            // one pair
            hand.type = HandType::ONE_PAIR;
            break;
        case 5:
            hand.type = HandType::HIGH_CARD;
        }

        ranking.push_back(&hand);

        // idea: build a long <TYPE><FIRST><SECOND><THIRD><FOURTH><FIFTH> and use that to sort
        long key = static_cast<long>(hand.type) * 100;
        for (auto it = hand.repr.cbegin(); it != hand.repr.cend(); it++) {
            char letter  = *it;
            key         += value_of.at(letter);
            key         *= 100;
        }

        hand.sort_key = key;
    }

    // sort the cards according to their sort key
    // ascending sort results in ranking[i] being rank i+1
    std::sort(ranking.begin(), ranking.end(), [](const Hand* lhs, const Hand* rhs) {
            return lhs->sort_key < rhs->sort_key;
            });

    int result = 0;
    for (std::size_t i = 0; i < ranking.size(); i++) {
        const Hand *hand = ranking[i];

        result += hand->bid * (i + 1);
    }

    infile.close();

    std::cout << "Number of total winnings is " << result << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << result << std::endl;
    outfile.close();
}
