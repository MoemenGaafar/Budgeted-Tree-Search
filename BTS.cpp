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

tuple<int, int> BTS(SlidingTilePuzzle* puzzle, bool uniform){

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

    vector<string>* currentState = puzzle->getInitialState();
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool done = false;
    bool found, bin_search_found;
    int total_expanded = 0, total_generated = 0, prev_expanded = 0;
    double nextf, goalCost, lastLowF;

    const int c1 = 2, c2 = 8;

    // BTS variables
    int n, budget_low, budget_high;
    double bin_search_low, bin_search_high, midF;

    bool new_found;
    int new_generated, new_expanded;
    double new_goal_cost, inside_time;

//    thisF = 39.705357142857146;
    while (!done){
        cout << "Doing Regular IDA* with f= " << thisF << endl;
        // Do regular DFS with infinite expansion budget
        tie(new_found, new_generated, new_expanded, nextf, new_goal_cost, inside_time) = DFS(puzzle, thisF, INF, uniform);
        goalCost = new_goal_cost;
        found = new_found;
        total_expanded += new_expanded;
        total_generated += new_generated;
        cout << "Expanded " << new_expanded << endl;
        if (found) break;

        // If no. of expanded nodes does not grow exponentially, go to EXP. search
        budget_low = c1 * prev_expanded;
        if (new_expanded >= budget_low){
            thisF = nextf;
            prev_expanded = new_expanded;
            continue;
        }

        budget_high = c2 * prev_expanded;
        n = 0;

        cout << "Doing EXP Search with low and high: " << budget_low << "    " << budget_high << endl;
        while (new_expanded < budget_low){
            // Break if goal is found while below budget
            if (found) break;
            lastLowF = thisF;
            thisF += pow(2, n);
            tie(new_found, new_generated, new_expanded, nextf, new_goal_cost, inside_time) = DFS(puzzle, thisF, budget_high, uniform);
            cout << "Expanded " << new_expanded << endl;
            goalCost = new_goal_cost;
            found = new_found;
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

        // If goal was found but we hit the budget limit, do regular IDA* using the same f-limit
        if (found){
            thisF = goalCost;
            prev_expanded = new_expanded;
            found = false;
            continue;
        }

        // If EXP. search hit the budget limit and goal not found, do BIN. search
        bin_search_low = lastLowF;
        bin_search_high = thisF;
        cout << "Doing BIN search with high and low: " << bin_search_high << "  " << bin_search_low << endl;

        bin_search_found = false;
        new_expanded = 0;
        while (bin_search_low < bin_search_high){
            midF = (bin_search_low + bin_search_high) / 2;
            cout << "midF " << midF << endl;
            tie(new_found, new_generated, new_expanded, nextf, new_goal_cost, inside_time) = DFS(puzzle, midF, budget_high, uniform);
            cout << "Expanded " << new_expanded << endl;
            goalCost = new_goal_cost;
            if (new_expanded <= budget_high && new_expanded >= budget_low){
                thisF = nextf;
                prev_expanded = new_expanded;
                break;
            }
            if (new_expanded > budget_high) bin_search_high = midF;
            if (new_expanded < budget_low) bin_search_low = midF;
//            if (abs(bin_search_high - bin_search_low) < EPSILON){
//                thisF = midF;
//                prev_expanded = new_expanded;
//                break;
//            }
        } 
    }
    cout << endl << "GOAL found with path of cost = " << goalCost << endl << endl;
    return {total_generated, total_expanded};
}