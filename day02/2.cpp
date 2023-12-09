#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

enum State { NONE, GAME_ID, NUMBER, COLOR };

int main() {
    std::ifstream infile("./input");

    // for every game check possibility
    std::string token;

    // state describes the thing we want to read
    State        state  = NONE;
    unsigned int sum    = 0;
    unsigned int amount = 0;

    std::map<std::string, unsigned int> min_amount = {
        {"blue",  0},
        {"green", 0},
        {"red",   0}
    };

    while (std::getline(infile, token)) {
        std::stringstream line(token);
        while (std::getline(line, token, ' ')) {
            // if the string starts with 'G' we found "Game"
            // the next token will be the game_id
            if (state == NONE && token[0] == 'G') {
                state = GAME_ID;

                int mul = 1;
                for (auto p : min_amount) {
                    mul                 *= p.second;
                    min_amount[p.first]  = 0;
                }

                sum += mul;

                continue;
            }

            std::istringstream iss(token);

            switch (state) {
            case GAME_ID:
                // game id will have the format <number>:
                state = NUMBER;
                break;
            case NUMBER:
                iss >> amount;
                state = COLOR;
                break;
            case COLOR:
                // when there is a ',' or ';' following the game goes on
                switch (token.back()) {
                case ',':
                case ';':
                    token.pop_back();
                    state = NUMBER;
                    break;
                default:
                    // after this we want to go back to the next "Game"
                    state = NONE;
                }

                if (amount > min_amount[token])
                    min_amount[token] = amount;
                break;
            default:
                break;
            }
        }
    }
    infile.close();

    // at the end of the line the varaibles are not accounted for so
    // do one last computation for the last lineint mul = 1;
    int mul = 1;
    for (auto p : min_amount)
        mul *= p.second;

    sum += mul;

    std::cout << "The sum of the IDs is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
