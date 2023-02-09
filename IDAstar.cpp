#include <iostream>
#include <array>
#include "Node.cpp"
#include "SlidingTilePuzzle.cpp"
using namespace std;

bool areIdenticalStates(vector<string> state1, vector<string> state2){
    for (int i=0; i<state1.size(); i++){
        if (state1.at(i) != state2.at(i)) return false;
    }
    return true;
}

bool isNotConsistent(Action action1, Action action2){
    if (abs(action1 - action2) != 1) return false;
    if ((action1 == DOWN && action2 == LEFT) || (action2 == DOWN && action1 == LEFT) ) return false;
    return true;
}

tuple<bool, int, int, double> DFS(SlidingTilePuzzle* puzzle, double flimit, double &nextf){
    Node currNode = Node(*puzzle->getInitialState());
    vector<Node> frontier;
    int num_generated = 0; int num_expanded = 0;
    frontier.push_back(currNode);
    nextf = 0;
    vector<string>* currState;
    vector<int> actions;
    vector<string> nextState;
    Node child;
    double childPathCost, child_fcost;
    double fcost;
    Action action, parentAction;
    double total_time = 0;
    time_t start, end;
    int i = 0;
    while (frontier.size() != 0){
        currNode = frontier.back();
        frontier.pop_back();
        currState = currNode.getStatePointer();
        fcost = currNode.getPathCost() + puzzle->getManHeuristic(currState);
        if (puzzle->isGoalReached(currState))
            return {true, num_generated, num_expanded, total_time};
        
        actions = {};

        puzzle->getActions(currState, &actions);
        parentAction = currNode.getLastAction();

        if (fcost < flimit) num_expanded += 1;

        start = clock();
        
        for (int a: actions){
            action = static_cast<Action>(a);
            if (fcost < flimit) num_generated += 1;
            if (isNotConsistent(action, parentAction)) continue;
            puzzle->doAction(currState, &nextState, action);
            childPathCost = currNode.getPathCost() + puzzle->getActionCost(currState, action);
            child_fcost = childPathCost + puzzle->getManHeuristic(&nextState);
            if (child_fcost > flimit){
                if (nextf == 0) nextf = child_fcost;
                else{
                    if (child_fcost < nextf) nextf = child_fcost;
                }
                continue;
            }
            child = Node(nextState, action, childPathCost);
            frontier.push_back(child);
        }
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        total_time += time_taken;
    }
    return {false, num_generated, num_expanded, total_time};
}

tuple<int, int> IDAstar(SlidingTilePuzzle* puzzle){
    Node initNode = Node(*puzzle->getInitialState());
    double thisF = initNode.getPathCost() + puzzle->getManHeuristic(initNode.getStatePointer());
    bool found = false;
    int total_expanded = 0, total_generated = 0;
    double nextf;
    while (!found){
        cout << "Searching at f-limit = " << thisF << endl;
        time_t start, end;
        start = clock();
        auto [found, new_generated, new_expanded, inside_time] = DFS(puzzle, thisF, nextf);
        end = clock();
        total_expanded += new_expanded;
        total_generated += new_generated;
        cout << "Expanded: " << total_expanded << " and Generated: " << total_generated << endl;
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by iteration: " << time_taken << " sec." << endl;
        cout << "Inside Time: " << inside_time << " sec." << endl;
        thisF = nextf;
    }
    return {total_generated, total_expanded};
}

