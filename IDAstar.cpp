#pragma once
#include <iostream>
#include <array>
#include "Node.cpp"
#include "SlidingTilePuzzle.cpp"
#include<map>
#include <limits>
#include <algorithm>
#include "DFS.cpp"
using namespace std;

tuple<int, int> IDAstar(SlidingTilePuzzle* puzzle, bool uniform){

    for (int i=0; i<2; i++){
        for (int j=0; j<2; j++){
            for (int k=0; k<2; k++){
                for (int l=0; l<2; l++){
                    ALLACTIONS[l*1+k*2+j*4+i*8] = array<bool, 4>{bool(l), bool(k), bool(j), bool(i)};
                }
            }
        }
    }

    for (int i=0; i<100; i++){
        FRONTIER.push_back(Node());
    }

    vector<string>* currentState = puzzle->getInitialState();
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool found = false;
    int total_expanded = 0, total_generated = 0;
    double nextf;
    double goalCost;
    while (!found){
        cout << "Searching at f-limit = " << thisF << endl;
        time_t start, end;
        start = clock();
        auto [new_found, new_generated, new_expanded, nextf, goalCost, inside_time] = DFS(puzzle, thisF, INF, uniform);
        found = new_found;
        end = clock();
        total_expanded += new_expanded;
        total_generated += new_generated;
        cout << "Expanded: " << total_expanded << " and Generated: " << total_generated << endl;
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by iteration: " << time_taken << " sec." << endl;
        cout << "Inside Time: " << inside_time << " sec." << endl;
        if (found) cout << endl << "GOAL found with path of cost = " << goalCost << endl << endl;
        thisF = nextf;
    }
    return {total_generated, total_expanded};
}

