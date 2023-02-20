#pragma once
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
        double fcost;
    
    public:
        Node(){
            pathCost = 0;
            lastAction = 0;
            fcost = 0;
        }

        Node(int aIndex, int sIndex){
            actionIndex = aIndex;
            pathCost = 0;
            lastAction = 0;
            spaceIndex = sIndex;
            fcost = 0;
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

        double getfcost(){
            return fcost;
        }

        double getHeuristic(){
            return fcost - pathCost;
        }

        void setLastAction(int action){
            lastAction = action;
        }

        void setNode(int aIndex, Action newParentAction, double newPathCost, int sIndex, double f){
            actionIndex = aIndex;
            parentAction = newParentAction;
            pathCost = newPathCost;
            lastAction = 0;
            spaceIndex = sIndex;
            fcost = f;
        }
        
        void setNode(int aIndex, int sIndex, double f){
            actionIndex = aIndex;
            pathCost = 0;
            lastAction = 0;
            spaceIndex = sIndex;
            fcost = f;
        }
};