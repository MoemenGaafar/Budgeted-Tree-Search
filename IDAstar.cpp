#include <iostream>
#include <array>
#include "Node.cpp"
#include "SlidingTilePuzzle.cpp"
#include<map>
using namespace std;

double EPSILON = 0.001;

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

map<int, array<bool, 4>*> ALLACTIONS;

tuple<bool, int, int, double> DFS(SlidingTilePuzzle* puzzle, double flimit, double &nextf){
    time_t start, end;
    vector<string>* currentState = puzzle->getInitialState();
    vector<Node*> frontier;
    int num_generated = 0; int num_expanded = 0;
//    array<bool, 4> initActions = {false, false, false, false};
    int spaceIndex = getIndex(currentState, "0");
    int actionIndex = puzzle->getActions(currentState, spaceIndex);
    array<bool, 4>* currentActions;
    array<bool, 4>* nextActions;
    Node initNode = Node(actionIndex, spaceIndex);
    Node* currentNode = &initNode;
    frontier.push_back(currentNode);
    Action action, lastAction, parentAction;
    Node* child;
    double childPathCost, child_fcost, actionCost;
    double fcost;
    int i;
    
    double total_time = 0;
    fcost = puzzle->getManHeuristic(currentState);
    if (flimit - fcost > EPSILON) num_expanded += 1;
    Node childNode;
    

    while (frontier.size() != 0){
        
        currentNode = frontier.back();
        currentActions = ALLACTIONS[currentNode->getActionIndex()];
//        cout << "Actions: ";
//        for (bool a: *currentActions){
//            cout << a << " ";
//        }
//        cout << endl;
        spaceIndex = currentNode->getSpaceIndex();
        if (currentNode->getLastAction() == 4){
            parentAction = currentNode->getParentAction();
            if (frontier.size() > 1){
                spaceIndex = puzzle->undoAction(currentState, parentAction, spaceIndex);
                currentNode->setSpaceIndex(spaceIndex);
            }
            frontier.pop_back();
            continue;
        }

        fcost = currentNode->getPathCost() + puzzle->getManHeuristic(currentState);

        if (puzzle->isGoalReached(currentState)){
            return {true, num_generated, num_expanded, total_time};
        }

        parentAction = currentNode->getParentAction();

        i = currentNode->getLastAction();
        while(i < 4){
//            cout << "action: " << i << " " << currentActions->at(i) << endl;
            if (!currentActions->at(i)){i++; continue;}
            action = static_cast<Action>(i);
            if (flimit - fcost > EPSILON) num_generated += 1;
            if (isNotConsistent(action, parentAction)) {i++; continue;}
            actionCost = puzzle->getActionCost(currentState, action);
//            start = clock();
            spaceIndex = puzzle->doAction(currentState, action, spaceIndex);
            i++;
            childPathCost = currentNode->getPathCost() + actionCost;
            child_fcost = childPathCost + puzzle->getManHeuristic(currentState);
//            end = clock();
//            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//            total_time += time_taken;
            if (child_fcost - flimit > EPSILON){
                if (nextf == 0) nextf = child_fcost;
                else{
                    if (nextf - child_fcost > EPSILON) nextf = child_fcost;
                }
                spaceIndex = puzzle->undoAction(currentState, action, spaceIndex);
                break;
            }
            actionIndex = puzzle->getActions(currentState, spaceIndex);
            child = new Node(actionIndex, action, childPathCost, spaceIndex);
            frontier.push_back(child);
            if (flimit - child_fcost > EPSILON) num_expanded+=1;
            break;
        }
        currentNode->setLastAction(i);
    }
    return {false, num_generated, num_expanded, total_time};
}

tuple<int, int> IDAstar(SlidingTilePuzzle* puzzle){
    for (int i=0; i<2; i++){
        for (int j=0; j<2; j++){
            for (int k=0; k<2; k++){
                for (int l=0; l<2; l++){
                    ALLACTIONS[l*1+k*2+j*4+i*8] = new array<bool, 4>{bool(l), bool(k), bool(j), bool(i)};
                }
            }
        }
    }
    vector<string>* currentState = puzzle->getInitialState();
    double thisF = puzzle->getManHeuristic(currentState);
    bool found = false;
    int total_expanded = 0, total_generated = 0;
    double nextf;
    while (!found){
        cout << "Searching at f-limit = " << thisF << endl;
        time_t start, end;
        start = clock();
        auto [new_found, new_generated, new_expanded, inside_time] = DFS(puzzle, thisF, nextf);
        found = new_found;
        end = clock();
        total_expanded += new_expanded;
        total_generated += new_generated;
        cout << "Expanded: " << total_expanded << " and Generated: " << total_generated << endl;
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
//        cout << "Time taken by iteration: " << time_taken << " sec." << endl;
//        cout << "Inside Time: " << inside_time << " sec." << endl;
        if (found) cout << endl << "GOAL found with path of cost = " << thisF << endl << endl;
        thisF = nextf;
        nextf = 0;
    }
    return {total_generated, total_expanded};
}

