#pragma once
#include <iostream>
#include <array>
#include "Node.cpp"
#include "SlidingTilePuzzle.cpp"
#include<map>
#include <limits>
#include <algorithm>
using namespace std;

const double EPSILON = 0.001;
const int INF = numeric_limits<int>::max();

int getIndex(vector<string> *myvec, string K)
{
    for (int i = 0; i < myvec->size(); i++){
        if (myvec->at(i) == K) return i;
    }
    return 0;
}

bool isNotConsistent(Action action1, Action action2){
    if (abs(action1 - action2) != 1) return false;
    if ((action1 == DOWN && action2 == LEFT) || (action2 == DOWN && action1 == LEFT) ) return false;
    return true;
}

bool allActionsDone(array<bool, 4>* actions){
    for (bool a: *actions){
        if (a) return false;
    }
    return true;
}

map<int, array<bool, 4>> ALLACTIONS;
vector<Node> FRONTIER;

tuple<bool, int, int, double, double, double> DFS(SlidingTilePuzzle* puzzle, double flimit, int nodeLimit, bool uniform){
    cout << "Starting DFS with f limit = " << flimit << endl;
    double nextf = 0, maxf = 0, foundGoalCost=INF;
    time_t start, end;
    vector<string>* currentState = puzzle->getInitialState();
//    vector<Node*> frontier;
    int frontierIndex = 0;
    vector<double> goalCosts;
    int num_generated = 0; int num_expanded = 0;
    int spaceIndex = getIndex(currentState, "0");
    int actionIndex = puzzle->getActions(currentState, spaceIndex);
    array<bool, 4>* currentActions;
    array<bool, 4>* nextActions;
//    Node initNode = Node(actionIndex, spaceIndex);
    
    FRONTIER[frontierIndex].setNode(actionIndex, spaceIndex);
    Node* currentNode = &FRONTIER[frontierIndex];

    Action action, lastAction, parentAction;
    Node* child;
    double childPathCost, child_fcost, actionCost;
    double fcost;
    int i, oldfrontierIndex;
    
    double total_time = 0;
    fcost = puzzle->getManHeuristic(currentState, uniform);
    if (flimit - fcost > EPSILON) num_expanded += 1;
    Node childNode;
    

    while (frontierIndex != -1){
//        start = clock();
        currentNode = &FRONTIER.at(frontierIndex);

        if (num_expanded > nodeLimit){
            currentNode->setLastAction(4);
        }

        parentAction = currentNode->getParentAction();
        spaceIndex = currentNode->getSpaceIndex();

//        end = clock();
//        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        total_time += time_taken;
        fcost = currentNode->getPathCost() + puzzle->getManHeuristic(currentState, uniform);

        if (currentNode->getLastAction() == 4 || fcost > foundGoalCost){
            if (frontierIndex > 0){
                spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
            }
//            frontier.pop_back();
            frontierIndex--;
            continue;
        }

        if (puzzle->isGoalReached(currentState)){
            if (nodeLimit == INF) return {true, num_generated, num_expanded, nextf, currentNode->getPathCost(), total_time};
            for (string a: *currentState){
                cout << a << " ";
            }
            cout << currentNode->getPathCost() << endl;
            if (currentNode->getPathCost() < foundGoalCost) foundGoalCost = currentNode->getPathCost();
            goalCosts.push_back(currentNode->getPathCost());
            spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
//            frontier.pop_back();
            frontierIndex--;
            continue;
        }
        
        if (fcost > maxf) maxf = fcost;
        i = currentNode->getLastAction();
        currentActions = &ALLACTIONS[currentNode->getActionIndex()];

        oldfrontierIndex = frontierIndex;
        while(i < 4){
            if (!currentActions->at(i)){i++; continue;}
            action = static_cast<Action>(i);
            if (flimit - fcost > EPSILON) num_generated += 1;
            if (isNotConsistent(action, parentAction)) {i++; continue;}
            actionCost = puzzle->getActionCost(currentState, action, spaceIndex, uniform);

            spaceIndex = puzzle->doAction(currentState, action, spaceIndex);
            i++;
            childPathCost = currentNode->getPathCost() + actionCost;
            child_fcost = childPathCost + puzzle->getManHeuristic(currentState, uniform);

            if (child_fcost - flimit > EPSILON){
                if (nextf == 0) nextf = child_fcost;
                else{
                    if (nextf - child_fcost > EPSILON) nextf = child_fcost;
                }
                spaceIndex = puzzle->undoAction(currentState, action, spaceIndex);
                continue;
            }
            actionIndex = puzzle->getActions(currentState, spaceIndex);
            frontierIndex++;
            
            FRONTIER[frontierIndex].setNode(actionIndex, action, childPathCost, spaceIndex);
            if (flimit - child_fcost > EPSILON) num_expanded+=1;
            break;
        }

        if (frontierIndex == oldfrontierIndex){
            if (frontierIndex > 0){
                spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
            }
            frontierIndex--;
            continue;
        }

        currentNode->setLastAction(i);

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

