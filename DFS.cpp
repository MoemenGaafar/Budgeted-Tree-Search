#pragma once
#include <iostream>
#include <array>
#include "Node.cpp"
#include "SlidingTilePuzzle.cpp"
#include<map>
#include <limits>
#include <algorithm>
using namespace std;

const double EPSILON = 1e-6;
const int INF = numeric_limits<int>::max();

double foundGoalCost=INF;

int getIndex(vector<int> *myvec, int K)
{
    for (int i = 0; i < myvec->size(); i++){
        if (myvec->at(i) == K) return i;
    }
    return 0;
}

bool isNotConsistent(SlidingTilePuzzle* puzzle, Action action1, Action action2){
    if (action1 == puzzle->getOppositeAction(action2)) return true;
    return false;
}

map<int, array<bool, 4>> ALLACTIONS;
vector<Node> FRONTIER;

tuple<bool, int, int, double, double, double> DFS(SlidingTilePuzzle* puzzle, double flimit, int nodeLimit, bool uniform){
    cout << "Starting DFS with f limit = " << flimit << endl;
    double nextf = 0, maxf = 0;
    time_t start, end;
    vector<int>* currentState = puzzle->getInitialState();
    int frontierIndex = 0;

    vector<double> goalCosts;
    int num_generated = 0; int num_expanded = 0;
    int spaceIndex = getIndex(currentState, 0);
    int actionIndex = puzzle->getActions(currentState, spaceIndex);
    array<bool, 4>* currentActions;
    array<bool, 4>* nextActions;
    
    Node* currentNode = &FRONTIER[frontierIndex];

    Action action, parentAction;
    Node* child;
    double childPathCost, child_fcost, actionCost, pathCost;
    double fcost, heuristic, child_heuristic;
    int lastAction, oldfrontierIndex;
    
    double total_time = 0;
    fcost = puzzle->getManHeuristic(currentState, uniform);
    FRONTIER[frontierIndex].setNode(actionIndex, spaceIndex, fcost);
    if (flimit - fcost > EPSILON) num_expanded += 1;
    Node childNode;
    

    while (frontierIndex != -1){
        currentNode = &FRONTIER.at(frontierIndex);

        if (num_expanded > nodeLimit){
            currentNode->setLastAction(4);
        }
        parentAction = currentNode->getParentAction();
        spaceIndex = currentNode->getSpaceIndex();
        lastAction = currentNode->getLastAction();
        fcost = currentNode->getfcost();
        
        if (lastAction == 4 || fcost > foundGoalCost){
            if (frontierIndex > 0){
                spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
            }
            frontierIndex--;
            continue;
        }
        
        pathCost = currentNode->getPathCost();

        if (puzzle->isGoalReached(currentState)){
            if (nodeLimit == INF) return {true, num_generated, num_expanded, nextf, pathCost, total_time};
            for (int a: *currentState){
                cout << a << " ";
            }
            cout << pathCost << endl;
            if (pathCost < foundGoalCost) foundGoalCost = pathCost;
            goalCosts.push_back(pathCost);
            spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
            frontierIndex--;
            continue;
        }

        heuristic = fcost - pathCost;

        if (fcost > maxf) maxf = fcost;
        currentActions = &ALLACTIONS[currentNode->getActionIndex()];
        oldfrontierIndex = frontierIndex;

        while(lastAction < 4){
            
            if (!currentActions->at(lastAction)){lastAction++; continue;}
            action = static_cast<Action>(lastAction);    
            
            if (flimit - fcost > EPSILON) num_generated += 1;
            if (isNotConsistent(puzzle, action, parentAction)) {lastAction++; continue;}

            actionCost = puzzle->getActionCost(currentState, action, spaceIndex, uniform);
            lastAction++;
            childPathCost = pathCost + actionCost;

//            start = clock();
            child_heuristic = puzzle->getNewHeuristic(currentState, heuristic, action, spaceIndex, uniform);
            child_fcost = childPathCost + child_heuristic;

//            end = clock();
//            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//            total_time += time_taken;

            if (child_fcost - flimit > EPSILON){
                if (nextf == 0) nextf = child_fcost;
                else{
                    if (nextf - child_fcost > EPSILON) nextf = child_fcost;
                }
                continue;
            }
            spaceIndex = puzzle->doAction(currentState, action, spaceIndex);
            actionIndex = puzzle->getActions(currentState, spaceIndex);
            frontierIndex++;
            FRONTIER[frontierIndex].setNode(actionIndex, action, childPathCost, spaceIndex, child_fcost);

            if (flimit - child_fcost > EPSILON) num_expanded+=1;

            break;
        }

        if (frontierIndex == oldfrontierIndex){
                if (frontierIndex > 0){
                    spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
                }
                frontierIndex--;
        }
        else
            currentNode->setLastAction(lastAction);

    }

    double goal;
    double cost, minCost;

    if (num_expanded > nodeLimit){
        nextf = maxf;
    }

    if (goalCosts.size() == 0)
        return {false, num_generated, num_expanded, nextf, 0, total_time};
    else{
        minCost = goalCosts.at(0);
        for (int i=0; i < goalCosts.size(); i++){
            goal = goalCosts.at(i);
            if (goal < minCost)
                minCost = goal;
        }
        return {true, num_generated, num_expanded, nextf, minCost, total_time};
    }
}

