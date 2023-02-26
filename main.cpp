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

void initializeLists(){
    for (int i=0; i<2; i++){
        for (int j=0; j<2; j++){
            for (int k=0; k<2; k++){
                for (int l=0; l<2; l++){
                    ALLACTIONS[l*1+k*2+j*4+i*8] = array<bool, 4>{bool(l), bool(k), bool(j), bool(i)};
                }
            }
        }
    }

    for (int i=0; i<1000; i++){
        FRONTIER.push_back(Node());
    }
}

int main(){
    initializeLists();
    // Read korf100 instances line by line
    ifstream KorfFile("korf100.txt");
    string korfInstance;
    int i = 1;
    time_t start, end;
    double time_taken;
    int total_generated, total_expanded;
    string output;
    vector<int> indices = {79, 42, 94, 12, 85, 19, 48, 31, 73};
    while (getline(KorfFile, korfInstance)){
        // Skip empty lines
        if (korfInstance.length() < 5)
            continue;
        
        if (!count(indices.begin(), indices.end(), i)) {i++; continue;}
//          if (i != 55) {i++; continue;}
//        if (i >= 75) break;
        int tabPosition = korfInstance.find("  ");
        string instanceId = korfInstance.substr(0, tabPosition);
        string positions = korfInstance.substr(tabPosition+3);
        cout << "\nSolving Instance #" << instanceId << endl;
        cout << positions << endl;

        // Create output files
//        ofstream IDAunit_outputFile("output/IDAstar_unit/instance_"+to_string(i)+".txt");
//        ofstream BTSunit_outputFile("output/BTS_unit/instance_"+to_string(i)+".txt");
//        ofstream BTSnonunit_outputFile("output/BTS_nonunit/instance_"+to_string(i)+".txt");

        // Initalize a new SlidingPuzzle instance for each line
        vector<int> positionsVector = splitString(positions);
        SlidingTilePuzzle puzzle(&positionsVector);

        // Call IDA* function to solve the puzzle instance with unit costs
//        cout << "Now doing IDA*" << endl;
//        output = "";
//        start = clock();
//        tie(total_generated, total_expanded, output) = IDAstar(&puzzle, true);
//        end = clock();
//        time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        output += "time_taken: " + to_string(time_taken) + " sec;";
//        cout << "Time taken: " << time_taken << endl;
//        IDAunit_outputFile << output << endl;

//        positionsVector = splitString(positions);
//        puzzle = SlidingTilePuzzle(&positionsVector);
//        // Call BTS function to solve the puzzle instance with unit costs and c1=2, c2=8
//        cout << "Now doing BTS with unit costs" << endl;
//        output = "";
//        start = clock();
//        tie(total_generated, total_expanded, output) = BTS(&puzzle, true, 2, 8);
//        end = clock();
//        time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        output += "time_taken: " + to_string(time_taken) + " sec;";
//        cout << "Time taken: " << time_taken << endl;
//        BTSunit_outputFile << output << endl;
//
//        positionsVector = splitString(positions);
//        puzzle = SlidingTilePuzzle(&positionsVector);
//        // Call BTS function to solve the puzzle instance with non-unit costs and c1=2, c2=8
//        cout << "Now doing BTS with non-unit costs, c1=2, c2=8" << endl;
//        output = "";
//        start = clock();
//        tie(total_generated, total_expanded, output) = BTS(&puzzle, false, 2, 8);
//        end = clock();
//        time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        output += "time_taken: " + to_string(time_taken) + " sec;";
//        cout << "Time taken: " << time_taken << endl;
//        BTSnonunit_outputFile << output << endl;
//
//        BTSnonunit_outputFile << endl << "----------------------------------------------------------------\n" << endl;
//
//        positionsVector = splitString(positions);
//        puzzle = SlidingTilePuzzle(&positionsVector);
//        // Call BTS function to solve the puzzle instance with non-unit costs and c1=c2=5
//        cout << "Now doing BTS with non-unit costs, c1=c2=5" << endl;
//        output = "";
//        start = clock();
//        tie(total_generated, total_expanded, output) = BTS(&puzzle, false, 5, 5);
//        end = clock();
//        time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        output += "time_taken: " + to_string(time_taken) + " sec;";
//        cout << "Time taken: " << time_taken << endl;
//        BTSnonunit_outputFile << output << endl;
//
//
//        IDAunit_outputFile.close();
//        BTSunit_outputFile.close();
//        BTSnonunit_outputFile.close();
//        i++;
//        continue; // Delete this line to run IDA* with non-unit costs

        ofstream IDAnonunit_outputFile("output/IDAstar_nonunit/instance_"+to_string(i)+".txt");

        positionsVector = splitString(positions);
        puzzle = SlidingTilePuzzle(&positionsVector);
        // Call IDA* function to solve the puzzle instance with non-unit costs
        start = clock();
        tie(total_generated, total_expanded, output) = IDAstar(&puzzle, false);
        end = clock();
        time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        output += "time_taken: " + to_string(time_taken) + " sec;";
        IDAnonunit_outputFile << output << endl;

        IDAnonunit_outputFile.close();
        i++;
    }

    KorfFile.close();
}