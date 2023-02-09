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

    int getActions(vector<string> *state, int spaceIndex)
    {
        int actionIndex = 0;
        if (spaceIndex / 4 > 0)
//              possibleActions->push_back(0);
//            possibleActions->at(0) = true;
        actionIndex+=1;
        if (spaceIndex / 4 < 3)
//         possibleActions->push_back(1);
//            possibleActions->at(1) = true;
        actionIndex+=2;
        if (spaceIndex % 4 > 0)
//         possibleActions->push_back(2);
//            possibleActions->at(2) = true;
        actionIndex+=4;
        if (spaceIndex % 4 < 3)
//         possibleActions->push_back(3);
//            possibleActions->at(3) = true;
        actionIndex+=8;
        return actionIndex;
    }

    double getActionCost(vector<string> *state, Action action)
    {
        return 1;
    }

    int doAction(vector<string> *state, Action action, int spaceIndex)
    {
        int swapIndex = getSwapIndex(spaceIndex, action);
//        cout << spaceIndex << " " << swapIndex << endl;
        state->at(spaceIndex) = state->at(swapIndex);
        state->at(swapIndex) = "0";
        return swapIndex;
    }

    Action getOppositeAction(Action action){
        if (action == UP) return DOWN;
        if (action == DOWN) return UP;
        if (action == LEFT) return RIGHT;
        else return LEFT;
    }

    int undoAction(vector<string> *state, Action action, int spaceIndex){
        Action oppositeAction = getOppositeAction(action);
        return doAction(state, oppositeAction, spaceIndex);
    }

    int getNextAction(Action action){
        return abs(action) + 1;
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