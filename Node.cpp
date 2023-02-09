#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node{
    private:
        Action parentAction;
        int lastAction;
        double pathCost;
        int actionIndex;
        int spaceIndex;
    
    public:
        Node(){
            pathCost = 0;
            lastAction = 0;
        }

        Node(int aIndex, int sIndex){
            actionIndex = aIndex;
            pathCost = 0;
            lastAction = 0;
            spaceIndex = sIndex;
        }

        Node(int aIndex, Action newParentAction, double newPathCost, int sIndex){
            actionIndex = aIndex;
            parentAction = newParentAction;
            pathCost = newPathCost;
            lastAction = 0;
            spaceIndex = sIndex;
        }
        
        int getSpaceIndex(){
            return spaceIndex;
        }

        void setSpaceIndex(int sIndex){
            spaceIndex = sIndex;
        }

        int getActionIndex(){
            return actionIndex;
        }

        Action getParentAction(){
            return parentAction;
        }

        int getLastAction(){
            return lastAction;
        }

        double getPathCost(){
            return pathCost;
        }

        void setLastAction(int action){
            lastAction = action;
        }
};