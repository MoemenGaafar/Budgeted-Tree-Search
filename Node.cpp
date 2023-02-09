#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node{
    private:
        Action lastAction;
        double pathCost;
        vector<string> state;
    
    public:
        Node(){
            pathCost = 0;
        }

        Node(vector<string> newState){
            state = newState;
            pathCost = 0;
        }

        Node(vector<string> newState, Action newAction, double newPathCost){
            state = newState;
            lastAction = newAction;
            pathCost = newPathCost;
        }

        vector<string> getState(){
            return state;
        }

        vector<string>* getStatePointer(){
            return &state;
        }

        Action getLastAction(){
            return lastAction;
        }

        double getPathCost(){
            return pathCost;
        }

        void printState(){
            for (string i: state){
                cout << i << ' ';
            }
            cout << endl;
        }
};