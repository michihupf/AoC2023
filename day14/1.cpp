#include <fstream>
#include <iostream>
#include <vector>

int main() {
    std::ifstream infile("./input");

    // read and parse the note

    std::string line;
    std::vector<int> o_count;
    std::vector<int> upwards_room;
    int y = 0;
    while (std::getline(infile, line)) {
        if (upwards_room.size() == 0)
            upwards_room.resize(line.size(), 0);

        int count = 0;
        for (int x = 0; x < (int)line.length(); x++) {
            char c = line[x];
            if (c == '.')
                upwards_room[x]++;
            else if (c == '#')
                upwards_room[x] = 0;
            else if (c == 'O')
                if (upwards_room[x] > 0)
                    o_count[y - upwards_room[x]]++;
                else {
                    count++;
                }
            else;
        }
        o_count.push_back(count);
        y++;
    }

    infile.close();

    int sum = 0;
    for (int i = 0; i < (int)o_count.size(); i++) {
        sum += o_count[i] * (o_count.size() - i);
    }

    std::cout << "Load is " << sum << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << sum << std::endl;
    outfile.close();
}
