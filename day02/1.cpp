#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

enum State { NONE, GAME_ID, NUMBER, COLOR };

int main() {
    const std::map<std::string, unsigned int> max_amount = {
        {"blue",  14},
        {"green", 13},
        {"red",   12}
    };

    ifstream infile("./input");

    // for every game check possibility
    string token;

    // state describes the thing we want to read
    State        state      = NONE;
    unsigned int sum        = 0;
    unsigned int game_id    = 0;
    unsigned int amount     = 0;
    bool         impossible = false;
    while (getline(infile, token)) {
        stringstream line(token);
        while (getline(line, token, ' ')) {
            // if the string starts with 'G' we found "Game"
            // the next token will be the game_id
            if (state == NONE && token[0] == 'G') {
                state = GAME_ID;

                // whenever we reach a "Game" token we know we processed the
                // last line
                if (!impossible)
                    sum += game_id;

                // make sure to reset everything for the next iteration
                impossible = false;
                continue;
            }

            istringstream iss(token);

            switch (state) {
            case GAME_ID:
                // game id will have the format <number>:
                iss >> game_id;
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

                if (amount > max_amount.at(token))
                    impossible = true;
                break;
            default:
                break;
            }
        }
    }
    infile.close();

    // at the end of the line the varaibles are not accounted for so
    // do one last computation for the last line
    if (!impossible)
        sum += game_id;

    cout << "The sum of the IDs is " << sum << endl;
    cout << "Writing result to ./output" << endl;

    ofstream outfile("./output");
    outfile << sum << endl;
    outfile.close();
}
