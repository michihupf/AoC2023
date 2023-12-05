#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// processes a map found in `infile`
// returns true if needs to be called again
bool process_map(ifstream &infile, vector<unsigned int> &initial, vector<unsigned int> &mapped) {
    string line;
    // no maps are identity
    mapped = initial;

    while (getline(infile, line)) {
        // when we reach \n we need to continue
        if (line == "")
            return true;

        // if we don't have a digit then this is the title - we don't care about it
        if (!isdigit(line[0]))
            continue;

        istringstream iss(line);
        unsigned int dest, source, range;
        iss >> dest >> source >> range;

        for (unsigned int i = 0; i < (unsigned int)initial.size(); i++) {
            if (initial[i] < source)
                continue;

            if (initial[i] > source + range)
                continue;

            // we are in the correct range now
            // offset from source is also offset from dest in mapped space
            unsigned int offset = initial[i] - source;

            mapped[i] = dest + offset;
        }
    }

    // when we went through the entire file we want to stop processing maps
    return false;
}


int main() {
    ifstream infile("./input");

    // line one are the seeds to be planted
    string line;
    getline(infile, line);

    vector<unsigned int> seeds;
    istringstream iss(line);
    unsigned int tmp;

    // seeds: 
    for (unsigned int i = 0; i < 6; i++)
        iss.get();

    while (!iss.eof()) {
        iss >> tmp;
        seeds.push_back(tmp);
    }

    vector<unsigned int> mapped;

    // now we do the mappings for every seed
    while (process_map(infile, seeds, mapped)) {
        swap(seeds, mapped);
    }

    // mapped now contains locations
    unsigned int min = mapped[0];
    for (unsigned int loc : mapped)
        if (loc < min)
            min = loc;
    
    infile.close();

    cout << "Lowest location numbe number is " << min << endl;
    cout << "Writing result to ./output" << endl;

    ofstream outfile("./output");
    outfile << min << endl;
    outfile.close();
}
