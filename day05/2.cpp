#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// usage Range(from, length)
using Range = std::pair<long long, long long>;

// processes a map found in `infile`
// returns true if needs to be called again
bool process_map(std::ifstream &infile, std::vector<Range> &initial, std::vector<Range> &mapped) {
    std::string line;
    // no maps are identity
    mapped = initial;

    // code works but performance is trash

    while (getline(infile, line)) {
        // when we reach \n we need to continue the next map, so tell the caller
        if (line == "")
            return true;

        // if we don't have a digit then this is the title - we don't care about it
        if (!isdigit(line[0]))
            continue;

        std::istringstream iss(line);
        long long dest, source, range;
        iss >> dest >> source >> range;

        long long source_end = source + range - 1;

        for (int i = 0; i < (int)initial.size(); i++) {
            // the logic will now differ from 1.cpp as we need to do entire
            // sequence checks

            long long start = initial[i].first;
            long long len = initial[i].second;

            long long end = start + len - 1;

            // first check if we have no overlap
            if (start > source_end || end < source)
                continue;

            long long min = std::max(source, start);
            long long min_offset = min - source;

            // remove initial to not overlap the mapped part
            mapped.erase(mapped.begin() + i);

            // we need to split the map into different parts now
            // <- initial -> <- mapped -> <- initial ->

            // always add the mapped range
            mapped.insert(mapped.begin() + i, Range(dest + min_offset, dest + min_offset + len));

            // when we have a partial overlap then add the <- initial -> parts
            if (min > start)
                mapped.push_back(Range(start, min - 1));
            if (source_end < end)
                mapped.push_back(Range(source_end + 1, end));
        }
    }

    // when we went through the entire file we want to stop processing maps
    return false;
}


int main() {
    std::ifstream infile("./example");

    // line one are the seeds to be planted
    std::string line;
    getline(infile, line);

    std::vector<Range> seeds;
    std::istringstream iss(line);

    // seeds: 
    iss.ignore(6);

    long long seed_start, seed_len;
    while (iss >> seed_start >> seed_len) {
        seeds.push_back(Range(seed_start, seed_len));
    }

    std::vector<Range> mapped;

    // now we do the mappings for every seed
    while (process_map(infile, seeds, mapped)) {
        if (mapped.size() > 0)
            swap(seeds, mapped);
    }

    // mapped now contains locations
    // the minumum location will always be the first element 
    // because we have ranges
    long long min = mapped.begin()->first;
    for (const auto &loc : mapped)
        if (loc.first < min)
            min = loc.first;
    
    infile.close();

    std::cout << "Lowest location numbe number is " << min << std::endl;
    std::cout << "Writing result to ./output" << std::endl;

    std::ofstream outfile("./output");
    outfile << min << std::endl;
    outfile.close();
}
