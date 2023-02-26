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
    if (action == UP)
        return spaceIndex - 4;
    else if (action == DOWN)
        return spaceIndex + 4;
    else if (action == LEFT)
        return spaceIndex - 1;
    else
        return spaceIndex + 1;
}

class SlidingTilePuzzle
{
  private:
    vector<int>* initialState;
  public:
    SlidingTilePuzzle(vector<int>* init){
        initialState = init;
    }
    vector<int>* getInitialState(){
        return initialState;
    }

    int getActions(vector<int> *state, int spaceIndex)
    {
        int actionIndex = 0;
        if (spaceIndex / 4 > 0) actionIndex+=1;
        if (spaceIndex / 4 < 3) actionIndex+=2;
        if (spaceIndex % 4 > 0) actionIndex+=4;
        if (spaceIndex % 4 < 3) actionIndex+=8;
        return actionIndex;
    }

    double getActionCost(vector<int> *state, Action action, int spaceIndex, bool uniform)
    {

        if (uniform) return 1;
        int swapIndex = getSwapIndex(spaceIndex, action);
        double tileNum = state->at(swapIndex);
        return (tileNum + 2) / (tileNum + 1);
    }

    int doAction(vector<int> *state, Action action, int spaceIndex)
    {
        int swapIndex = getSwapIndex(spaceIndex, action);
        state->at(spaceIndex) = state->at(swapIndex);
        state->at(swapIndex) = 0;
        return swapIndex;
    }

    Action getOppositeAction(Action action){
        if (action == UP) return DOWN;
        if (action == DOWN) return UP;
        if (action == LEFT) return RIGHT;
        else return LEFT;
    }

    int undoAction(vector<int> *state, Action action, int spaceIndex){
        return doAction(state, getOppositeAction(action), spaceIndex);
    }

    bool isGoalReached(vector<int> *state){
        for (int i = 0; i < 16; i++){
            if (state->at(i) != i){
                return false;
            }
        }
        return true;
    }

    double getManHeuristic(vector<int> *state, bool uniform){
        double distance = 0, extraDistance;
        int element;
        for (int i=0; i<16; i++){
            element = state->at(i);
            if (element == 0) continue;
            extraDistance = 0;
            extraDistance += abs(element%4 - i%4);
            extraDistance += abs(element/4 - i/4);
            if (!uniform){ extraDistance *= (element + 2.0) / (element + 1.0);}
            distance += extraDistance;
        }
        return distance;
    }

    double getNewHeuristic(vector<int> *state, double oldHeuristic, Action action, int spaceIndex, bool uniform){
        int swapIndex = getSwapIndex(spaceIndex, action);
        int element = state->at(swapIndex);
        double difference;
        if (action == LEFT || action == RIGHT){
            difference = abs(element%4 - spaceIndex%4) - abs(element%4 - swapIndex%4);
        }
        else{
            difference = abs(element/4 - spaceIndex/4) - abs(element/4 - swapIndex/4);
        }

        if (!uniform){ difference *= (element + 2.0) / (element + 1.0);}

        return oldHeuristic + difference;
    }
};