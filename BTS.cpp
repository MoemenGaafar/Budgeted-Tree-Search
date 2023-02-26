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

tuple<long, long, string> BTS(SlidingTilePuzzle* puzzle, bool uniform, int c1, int c2){

    string output = "Performing BTS with c1 = "+to_string(c1)+" and c2 = "+to_string(c2)+"\n";

    foundGoalCost = INF;

    vector<int>* currentState = puzzle->getInitialState();
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool found, bin_search_found;
    long total_expanded = 0, total_generated = 0, budget_low, budget_high;
    double nextf, goalCost, lastLowF;

    // BTS variables
    int n;
    double bin_search_low, bin_search_high, midF;

    long new_generated, new_expanded, prev_expanded = 0;
    double inside_time;

//    thisF = 39.705357142857146;
    while (!found){
        output += "\nDoing Regular IDA* with f-limit = " + to_string(thisF) + "; ";
        cout << "\nDoing Regular IDA* with f-limit = " + to_string(thisF) + "; ";
        // Do regular DFS with infinite expansion budget
        tie(found, new_generated, new_expanded, nextf, goalCost, inside_time) = DFS(puzzle, thisF, INF, uniform);
        total_expanded += new_expanded;
        total_generated += new_generated;
        output += "Expanded " + to_string(new_expanded) + "; Generated " + to_string(new_generated) + ";\n";
        cout << "Expanded " + to_string(new_expanded) + "; Generated " + to_string(new_generated) + ";\n";
        if (found) break;

        // If no. of expanded nodes does not grow exponentially, go to EXP. search
        budget_low = c1 * prev_expanded;
        if (new_expanded >= budget_low){
            thisF = nextf;
            prev_expanded = new_expanded;
            continue;
        }

        budget_high = c2 * prev_expanded;
        n = 1;

        output += "\nDoing EXP Search with interval: [" + to_string(budget_low) + ", " + to_string(budget_high) + "]\n";
        cout << "\nDoing EXP Search with interval: [" + to_string(budget_low) + ", " + to_string(budget_high) + "]\n";
        while (new_expanded < budget_low){
            // Break if goal is found while below budget
            if (found) break;
            lastLowF = thisF;
            thisF += pow(2, n);
            output += "Searching with f-limit = " + to_string(thisF) + "; ";
            cout << "Searching with f-limit = " + to_string(thisF) + "; ";
            tie(found, new_generated, new_expanded, nextf, goalCost, inside_time) = DFS(puzzle, thisF, budget_high, uniform);
            total_expanded += new_expanded;
            total_generated += new_generated;
            output += "Expanded " + to_string(new_expanded) + "; Generated " + to_string(new_generated) + ";\n";
            n++;
        }

        // If we are still within the budget
        if (new_expanded <= budget_high){
            // Break if goal was found within budget
            if (found) break;

            // Go back to IDA* otherwise
            thisF = nextf;
            prev_expanded = new_expanded;
            continue;
        }


        // If EXP. search hit the budget limit and goal not found, do BIN. search
        bin_search_low = lastLowF;
        bin_search_high = nextf;
        output += "\nDoing BIN search with interval: ["+ to_string(bin_search_low) + ", " + to_string(bin_search_high) + "]\n";
        bin_search_found = false;
        new_expanded = 0;
        while (bin_search_low < bin_search_high){
            cout << "\nDoing BIN search with interval: ["+ to_string(bin_search_low) + ", " + to_string(bin_search_high) + "]\n";
            midF = (bin_search_low + bin_search_high) / 2;
            output += "Searching with f-limit = " + to_string(midF) + "; ";
            cout << "Searching with f-limit = " + to_string(midF) + "; ";
            tie(found, new_generated, new_expanded, nextf, goalCost, inside_time) = DFS(puzzle, midF, budget_high, uniform);
            total_expanded += new_expanded;
            total_generated += new_generated;
            output += "Expanded " + to_string(new_expanded) + "; Generated " + to_string(new_generated) + ";\n";

            if (found) break;

            if (new_expanded <= budget_high && new_expanded >= budget_low){
                thisF = nextf;
                prev_expanded = new_expanded;
                break;
            }

            if (new_expanded > budget_high) bin_search_high = midF;
            if (new_expanded < budget_low) bin_search_low = midF;
            if (abs(bin_search_high - bin_search_low) < 0.1){
                thisF = midF;
                prev_expanded = new_expanded;
                break;
            }

        } 
    }

    output += "\nGOAL found with path of cost = " + to_string(goalCost)  + "\n";
    output += "Summary: total_expanded " + to_string(total_expanded) + ";total_generated " + to_string(total_generated)
            + ";goal_cost " + to_string(goalCost) + "; ";

    return {total_generated, total_expanded, output};
}