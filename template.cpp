#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream infile("./input");

    int sum = 0;
    // CODE GOES HERE
    
    infile.close();

    cout << "MESSAGE" << sum << endl;
    cout << "Writing result to ./output" << endl;

    ofstream outfile("./output");
    outfile << sum << endl;
    outfile.close();
}
