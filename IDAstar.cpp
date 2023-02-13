#include <iostream>
#include <array>
#include "Node.cpp"
#include "SlidingTilePuzzle.cpp"
#include<map>
#include <limits>
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

map<int, array<bool, 4>*> ALLACTIONS;

tuple<bool, int, int, double, double, double> DFS(SlidingTilePuzzle* puzzle, double flimit, int nodeLimit, bool uniform){
    double nextf = 0, maxf = 0;
    time_t start, end;
    vector<string>* currentState = puzzle->getInitialState();
    vector<Node*> frontier;
    vector<Node*> goals;
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
    fcost = puzzle->getManHeuristic(currentState, uniform);
    if (flimit - fcost > EPSILON) num_expanded += 1;
    Node childNode;
    

    while (frontier.size() != 0){
        if (num_expanded > nodeLimit){
            if (goals.size() == 0)
                return {false, num_generated, num_expanded, maxf, 0, total_time};
            else
                return {true, num_generated, num_expanded, maxf, goals.back()->getPathCost(), total_time};
        }
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

        fcost = currentNode->getPathCost() + puzzle->getManHeuristic(currentState, uniform);
        if (fcost > maxf) maxf = fcost;

        if (puzzle->isGoalReached(currentState)){
            if (nodeLimit == INF) return {true, num_generated, num_expanded, nextf, currentNode->getPathCost(), total_time};

            goals.push_back(currentNode);
            frontier.pop_back();
            continue;
        }

        parentAction = currentNode->getParentAction();

        i = currentNode->getLastAction();
        while(i < 4){
//            cout << "action: " << i << " " << currentActions->at(i) << endl;
            if (!currentActions->at(i)){i++; continue;}
            action = static_cast<Action>(i);
            if (flimit - fcost > EPSILON) num_generated += 1;
            if (isNotConsistent(action, parentAction)) {i++; continue;}
            actionCost = puzzle->getActionCost(currentState, action, spaceIndex, uniform);
//            start = clock();
            spaceIndex = puzzle->doAction(currentState, action, spaceIndex);
            i++;
            childPathCost = currentNode->getPathCost() + actionCost;
            child_fcost = childPathCost + puzzle->getManHeuristic(currentState, uniform);
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

    Node* goal;
    double cost, minCost;
    if (goals.size() == 0)
        return {false, num_generated, num_expanded, nextf, 0, total_time};
    else{
        minCost = goals.at(0)->getPathCost();
        for (int i=0; i < goals.size(); i++){
            goal = goals.back();
            if (goal->getPathCost() < minCost)
                minCost = goal->getPathCost();
            goals.pop_back();
        }
        return {true, num_generated, num_expanded, nextf, minCost, total_time};
    }

}

tuple<int, int> IDAstar(SlidingTilePuzzle* puzzle, bool uniform){
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
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool found = false;
    int total_expanded = 0, total_generated = 0;
    double nextf;
    double goalCost;
    while (!found){
        cout << "Searching at f-limit = " << thisF << endl;
        time_t start, end;
        start = clock();
        auto [new_found, new_generated, new_expanded, nextf, goalCost, inside_time] = DFS(puzzle, thisF, INF, uniform);
        found = new_found;
        end = clock();
        total_expanded += new_expanded;
        total_generated += new_generated;
        cout << "Expanded: " << total_expanded << " and Generated: " << total_generated << endl;
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by iteration: " << time_taken << " sec." << endl;
        cout << "Inside Time: " << inside_time << " sec." << endl;
        if (found) cout << endl << "GOAL found with path of cost = " << goalCost << endl << endl;
        thisF = nextf;
    }
    return {total_generated, total_expanded};
}



tuple<int, int> BTS(SlidingTilePuzzle* puzzle, bool uniform){
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
    double thisF = puzzle->getManHeuristic(currentState, uniform);
    bool done = false;
    bool found, bin_search_found;
    int total_expanded = 0, total_generated = 0, prev_expanded = 0;
    double nextf, goalCost, lastLowF;

    const int c1 = 2, c2 = 8;

    // BTS variables
    int n, budget_low, budget_high;
    double bin_search_low, bin_search_high, midF;

    while (!done){
        // Do regular DFS with infinite expansion budget
        auto [new_found, new_generated, new_expanded, nextf, new_goal_cost, inside_time] = DFS(puzzle, thisF, INF, uniform);
        goalCost = new_goal_cost;
        found = new_found;
        total_expanded += new_expanded;
        total_generated += new_generated;
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

        while (new_expanded < budget_low){
            // Break if goal is found while below budget
            if (found) break;
            lastLowF = thisF;
            thisF += pow(2, n);
            auto [new_found, new_generated, new_expanded, nextf, new_goal_cost, inside_time] = DFS(puzzle, thisF, budget_high, uniform);
            goalCost = new_goal_cost;
            found = new_found;
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
            thisF = thisF;
            prev_expanded = new_expanded;
            found = false;
            continue;
        }

        // If EXP. search hit the budget limit and goal not found, do BIN. search
        bin_search_low = lastLowF;
        bin_search_high = thisF;
        bin_search_found = false;
        while (bin_search_low < bin_search_high){
            midF = (bin_search_low + bin_search_high) / 2;
            auto [new_found, new_generated, new_expanded, nextf, new_goal_cost, inside_time] = DFS(puzzle, midF, budget_high, uniform);
            goalCost = new_goal_cost;
            if (new_expanded <= budget_high && new_expanded >= budget_low){
                nextf = midF;
                prev_expanded = new_expanded;
                break;
            }
            if (new_expanded > budget_high) bin_search_high = midF;
            if (new_expanded < budget_low) bin_search_low = midF;
        } 
    }
    cout << endl << "GOAL found with path of cost = " << goalCost << endl << endl;
    return {total_generated, total_expanded};
}

