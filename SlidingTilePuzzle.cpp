#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

enum Action
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int getIndex(vector<string> *myvec, string K)
{
    for (int i = 0; i < myvec->size(); i++){
        if (myvec->at(i) == K) return i;
    }
    return 0;
}

int getSwapIndex(int spaceIndex, Action action)
{
    int swapIndex = 0;
    if (action == UP)
        swapIndex = spaceIndex - 4;
    else if (action == DOWN)
        swapIndex = spaceIndex + 4;
    else if (action == LEFT)
        swapIndex = spaceIndex - 1;
    else if (action == RIGHT)
        swapIndex = spaceIndex + 1;

    return swapIndex;
}

class SlidingTilePuzzle
{
  private:
    vector<string>* initialState;
  public:
    SlidingTilePuzzle(vector<string>* init){
        initialState = init;
    }
    vector<string>* getInitialState(){
        return initialState;
    }

    void getActions(vector<string> *state, vector<int> *possibleActions)
    {
        int spaceIndex = getIndex(state, "0");
        if (spaceIndex / 4 > 0)
              possibleActions->push_back(0);
//            possibleActions->at(0) = true;
        if (spaceIndex / 4 < 3)
         possibleActions->push_back(1);
//            possibleActions->at(1) = true;
        if (spaceIndex % 4 > 0)
         possibleActions->push_back(2);
//            possibleActions->at(2) = true;
        if (spaceIndex % 4 < 3)
         possibleActions->push_back(3);
//            possibleActions->at(3) = true;
    }

    double getActionCost(vector<string> *state, Action action)
    {
        return 1;
    }

    void doAction(vector<string> *state, vector<string> *newState, Action action)
    {
        int spaceIndex = getIndex(state, "0");
        int swapIndex = getSwapIndex(spaceIndex, action);
        *newState = *state;
        newState->at(swapIndex) = state->at(spaceIndex);
        newState->at(spaceIndex) = state->at(swapIndex);
    }

    bool isGoalReached(vector<string> *state){
        for (int i = 0; i < 16; i++){
            if (state->at(i) != to_string(i)){
                return false;
            }
        }
        return true;
    }

    int getManHeuristic(vector<string> *state){
        int distance = 0;
        for (int i=0; i<16; i++){
            int element = stoi(state->at(i));
            if (element == 0) continue;
            distance += abs(element%4 - i%4);
            distance += abs(element/4 - i/4);
        }
        return distance;
    }
};