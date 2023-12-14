#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using ull = unsigned long long;

struct Point {
    int x, y;
};

template <bool North = true> void do_vertical_push(std::vector<std::string> &note) {
    std::vector<int> upwards_room(note[0].length(), 0);

    const int       start = North ? 0 : (int)note.size() - 1;
    const int       end   = North ? (int)note.size() : -1;
    constexpr short step  = North * 2 - 1;

    for (int y = start; y != end; y += step) {
        const std::string &str = note[y];
        for (int x = 0; x < (int)str.length(); x++) {
            switch (str[x]) {
            case '.':
                upwards_room[x] += step;
                break;
            case '#':
                upwards_room[x] = 0;
                break;
            case 'O':
                std::swap(note[y - upwards_room[x]][x], note[y][x]);
            }
        }
    }
}

template <bool West = true> void do_horizontal_push(std::vector<std::string> &note) {
    const int       start = West ? 0 : (int)note.size() - 1;
    const int       end   = West ? (int)note.size() : -1;
    constexpr short step  = West * 2 - 1;

    for (std::string &str : note) {
        int sideways_room = 0;
        for (int x = start; x != end; x += step) {
            switch (str[x]) {
            case '.':
                sideways_room += step;
                break;
            case '#':
                sideways_room = 0;
                break;
            case 'O':
                std::swap(str[x - sideways_room], str[x]);
            }
        }
    }
}

int main() {
    std::ifstream infile("./example");

    // read and parse the note

    std::vector<std::string> note;
    // stores the immovable rocks
    std::vector<Point> walls;
    // stores the movable rocks
    std::vector<Point> rocks;
    std::string        line;
    int                y = 0;
    while (std::getline(infile, line)) {
        // for (int x = 0; x < (int)line.length(); x++) {
        //     char c = line[x];
        //     if (c == '#')
        //         walls.push_back({x, y});
        //     else if (c == 'O')
        //         rocks.push_back({x, y});
        // }
        // y++;
        note.push_back(line);
    }

    infile.close();

    // do the 1'000'000'000 iterations of cycles
    for (ull i = 0; i < 1'000'000; i++) {
        do_vertical_push<true>(note);    // north
        do_horizontal_push<true>(note);  // west
        do_vertical_push<false>(note);   // south
        do_horizontal_push<false>(note); // east

        if (i % 100'000 == 0)
            std::cout << "Iteration: " << i << std::endl;
    }

    for (std::string &str : note)
        std::cout << str << std::endl;

    std::vector<int> upwards_room(note[0].length(), 0);
    std::vector<int> o_count;
    for (int y = 0; y < (int)note.size(); y++) {
        const std::string &str   = note[y];
        int                count = 0;
        for (int x = 0; x < (int)str.length(); x++) {
            char c = str[x];
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
            else
                ;
        }
        o_count.push_back(count);
    }

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
