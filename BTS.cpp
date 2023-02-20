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

tuple<int, int> BTS(SlidingTilePuzzle* puzzle, bool uniform, int c1, int c2){

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

    vector<int>* currentState = puzzle->getInitialState();
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool found, bin_search_found;
    int total_expanded = 0, total_generated = 0, prev_expanded = 0;
    double nextf, goalCost, lastLowF;

    // BTS variables
    int n, budget_low, budget_high;
    double bin_search_low, bin_search_high, midF;

    int new_generated, new_expanded;
    double inside_time;

//    thisF = 39.705357142857146;
    while (!found){
        cout << "Doing Regular IDA* with f= " << thisF << endl;
        // Do regular DFS with infinite expansion budget
        tie(found, new_generated, new_expanded, nextf, goalCost, inside_time) = DFS(puzzle, thisF, INF, uniform);
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
        n = 1;

        cout << "Doing EXP Search with low and high: " << budget_low << "    " << budget_high << endl;
        while (new_expanded < budget_low){
            // Break if goal is found while below budget
            if (found) break;
            lastLowF = thisF;
            thisF += pow(2, n);
            tie(found, new_generated, new_expanded, nextf, goalCost, inside_time) = DFS(puzzle, thisF, budget_high, uniform);
            total_expanded += new_expanded;
            total_generated += new_generated;
            cout << "Expanded " << new_expanded << endl;
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
//        if (found){
//            thisF = goalCost;
//            prev_expanded = new_expanded;
//            found = false;
//            continue;
//        }

        // If EXP. search hit the budget limit and goal not found, do BIN. search
        bin_search_low = lastLowF;
        bin_search_high = thisF;
        cout << "Doing BIN search with high and low: " << bin_search_high << "  " << bin_search_low << endl;

        bin_search_found = false;
        new_expanded = 0;
        while (bin_search_low < bin_search_high){
            midF = (bin_search_low + bin_search_high) / 2;
            cout << "midF " << midF << endl;
            tie(found, new_generated, new_expanded, nextf, goalCost, inside_time) = DFS(puzzle, midF, budget_high, uniform);
            total_expanded += new_expanded;
            total_generated += new_generated;
            cout << "Expanded " << new_expanded << endl;
            if (new_expanded <= budget_high && new_expanded >= budget_low){
                thisF = nextf;
                prev_expanded = new_expanded;
                cout << "Expanded exactly!" << endl;
                break;
            }

//            if (found){
//                thisF = goalCost;
//                prev_expanded = new_expanded;
//                found = false;
//                continue;
//            }

            if (new_expanded > budget_high) bin_search_high = midF;
            if (new_expanded < budget_low) bin_search_low = midF;
            if (abs(bin_search_high - bin_search_low) < 0.1){
                thisF = midF;
                prev_expanded = new_expanded;
                cout << "Defaulted!" << endl;
                break;
            }

        } 
    }
    cout << endl << "GOAL found with path of cost = " << goalCost << endl << endl;
    return {total_generated, total_expanded};
}