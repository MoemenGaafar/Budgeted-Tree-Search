#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include <bits/stdc++.h>

#include "SlidingTilePuzzle.cpp"
#include "IDAstar.cpp"
using namespace std;

std::vector<std::string> splitString(const std::string &str) {
    std::stringstream ss(str);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> list(begin, end);
    return list;
}

int main(){
    // Read korf100 instances line by line
    ifstream KorfFile("korf100.txt");
    string korfInstance;
    int i = 0;
    while (getline(KorfFile, korfInstance)){
        if (korfInstance.length() < 5)
            continue;
        int tabPosition = korfInstance.find("  ");
        string instanceId = korfInstance.substr(0, tabPosition);
        string positions = korfInstance.substr(tabPosition+3);
//        string positions = "1 0 2 3 4 5 6 7 8 9 10 11 12 13 14 15";
        cout << "Solving Instance #" << instanceId << endl;
        cout << positions << endl;
        // Initalize a new SlidingPuzzle instance for each line
        vector<string> positionsVector = splitString(positions);
        SlidingTilePuzzle puzzle(&positionsVector);
        time_t start, end;
        start = clock();
//        auto [total_generated, total_expanded] = IDAstar(&puzzle, true);
        auto [total_generated, total_expanded] = BTS(&puzzle, true);
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken: " << time_taken << " sec." << endl << endl;
        i++;

        break;
        // Call IDA* function to solve the puzzle instance
    }

    KorfFile.close();
}