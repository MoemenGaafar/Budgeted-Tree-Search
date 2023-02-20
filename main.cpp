#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include <bits/stdc++.h>

#include "SlidingTilePuzzle.cpp"
#include "IDAstar.cpp"
#include "BTS.cpp"
using namespace std;

vector<int> splitString(const std::string &str) {
    stringstream ss(str);
    istream_iterator<std::string> begin(ss);
    istream_iterator<std::string> end;
    vector<string> list(begin, end);
    vector<int> intList;
    for (string value: list){
        intList.push_back(stoi(value));
    }
    return intList;
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
        cout << "Solving Instance #" << instanceId << endl;
        cout << positions << endl;
        // Initalize a new SlidingPuzzle instance for each line
        vector<int> positionsVector = splitString(positions);
        SlidingTilePuzzle puzzle(&positionsVector);
        time_t start, end;
        start = clock();
//      auto [total_generated, total_expanded] = IDAstar(&puzzle, true);
        auto [total_generated, total_expanded] = BTS(&puzzle, false, 2, 8);
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken: " << time_taken << " sec." << endl << endl;
        i++;

        // Call IDA* function to solve the puzzle instance
        break;
    }

    KorfFile.close();
}