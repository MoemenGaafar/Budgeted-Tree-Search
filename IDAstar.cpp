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

tuple<long, long, string> IDAstar(SlidingTilePuzzle* puzzle, bool uniform){
    
    foundGoalCost = INF;

    string output = "";

    vector<int>* currentState = puzzle->getInitialState();
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool found = false;
    long total_expanded = 0, total_generated = 0;
    double nextf;
    double goalCost;
    while (!found){
        output += "Searching at f-limit = " + to_string(thisF) + "; ";
//        time_t start, end;
//        start = clock();
        auto [new_found, new_generated, new_expanded, nextf, goalCost, inside_time] = DFS(puzzle, thisF, INF, uniform);
        found = new_found;
//        end = clock();
        total_expanded += new_expanded;
        total_generated += new_generated;
        output += "Expanded " + to_string(total_expanded) + "; Generated " + to_string(total_generated) + ";\n";
//        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        cout << "Time taken by iteration: " << time_taken << " sec." << endl;
//        cout << "Inside Time: " << inside_time << " sec." << endl;
        if (found){
        output += "\nGOAL found with path of cost = " + to_string(goalCost) + "\n";
        output += "Summary: total_expanded " + to_string(total_expanded) + ";total_generated " + to_string(total_generated)
            + ";goal_cost " + to_string(goalCost) + "; ";
        }
        thisF = nextf;
    }
    return {total_generated, total_expanded, output};
}

