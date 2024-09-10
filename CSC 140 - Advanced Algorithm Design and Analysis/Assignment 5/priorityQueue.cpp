// © 2024 IllyaGY
// Licensed under the MIT License. See the LICENSE file for details.

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>


struct Node{
    int nodeNum = -1;
    int nodeValue = NULL;
};

class PriorityQueue{
    Node *queue = nullptr;
    int *indexTrack = nullptr;          //Tracks all queue nodes in the tree
    int size;                           //size of the queue
    int maxDistance;                    //The sum of edge distance + 1
    int lastIndex;                      //Tracks the last non-emtpy element in the tree

public:
    PriorityQueue(int size, int maxDistance){
        this->size = size;
        this->maxDistance = maxDistance;
        this->lastIndex = -1;
        queue = new Node [size];
        indexTrack = new int [size];


        for(int i = 0; i < size; i++) {             //Fills out the indexes with -1 and queue with nodes of distance=maxDistance
            Node node = {-1, maxDistance};
            indexTrack[i] = -1;
            queue[i] = node;
        }

    }

    ~PriorityQueue(){
        delete [] queue;
        delete [] indexTrack;
    }

    int sizeQ(){
        return size;
    }


    void swapNodes(int currentNode, int other, Node newNode){                                    //Moves the last NON-Empty element to the root

        indexTrack[queue[other].nodeNum] = indexTrack[queue[currentNode].nodeNum];
        queue[currentNode] = queue[other];
        queue[other] = newNode;
        lastIndex--;


    }

    void indexUpdate(int nodeIndex, int &queueIndex, int nodeIndex1 = -1, int queueIndex1 = -1){    //Updates indexes after swapping a parent and its child
        indexTrack[nodeIndex] = queueIndex;
        if(nodeIndex1 != -1 && queueIndex1 != -1) {
            indexTrack[nodeIndex1] = queueIndex1;
            queueIndex = queueIndex1;
        }
    }


    Node extractMin(){                                      /*Extracts the root, and assigns the new root based on the least value left in the queue*/
        Node root = queue[0];                                                   //Saves the root before extracting
        int index = 0, indexNbr = 0;

        swapNodes(0, lastIndex, Node {-1, maxDistance});


        Node *smallest = nullptr;

        if (queue[2*index+1].nodeValue<=queue[2*index+2].nodeValue){             //Check which one of the two children on a node is the smallest
            smallest = &queue[2*index+1];   //Left branch
            indexNbr = 2 * index + 1;
        }
        else{
            smallest = &queue[2*index+2];   //Right branch
            indexNbr = 2 * index + 2;
        }



        while (indexNbr < sizeQ() && queue[index].nodeValue > (*smallest).nodeValue) {          //Moves the new root node lower down the tree if any of its children have smaller values
            std::swap(queue[index], *smallest);
            indexTrack[queue[index].nodeNum] = index;
            index = indexNbr;
            if (queue[2*index+1].nodeValue<=queue[2*index+2].nodeValue){
                smallest = &queue[2*index+1];
                indexNbr = 2 * index + 1;
            }
            else{
                smallest = &queue[2*index+2];
                indexNbr = 2 * index + 2;
            }


        }

        return root;
    }
    void update(int nodeNum, int value){
        int index = indexTrack[nodeNum];
        if (index == -1){                                       //If a node with a such number doesn't exist add it to the end
            lastIndex++;
            queue[lastIndex] = Node {nodeNum, value};
            indexUpdate(nodeNum, lastIndex);
            index = indexTrack[nodeNum];
        }
        else if (queue[index].nodeValue > value) {              //If it exists, check if the new value is less than the previous value
            queue[index].nodeValue = value;

        }
        while(queue[index].nodeValue < queue[(index-1)/2].nodeValue){      //If a parent of this node has a greater value, swap the nodes
            std::swap(queue[(index-1) / 2], queue[index]);
            indexUpdate(queue[index].nodeNum, index, nodeNum, (index-1)/2);

        }
    }
    Node getRoot(){
        return queue[0];
    }

    void printN(){                                                  //Prints out a tree (used for debugging purposes)
        std::string space;
        int s = sizeQ();
        space.insert(0, sizeQ(), ' ');
        int treeN = 1;
        int next = treeN;
        for(int i = 0; i <= lastIndex; i++){
            if(queue[i].nodeValue >= maxDistance)std::cout << space << "-:-" << space;
            else std::cout << space << queue[i].nodeNum << ":" << queue[i].nodeValue << space;
            treeN--;
            if(treeN == 0){
                next*=2;
                treeN = next;
                for(int i = 0; i < s/next; i++)
                    space.erase(space.begin()+i);
                std::cout << "\n";
            }
        }

        std::cout << "\n";
        std::cout << "---------------------------";
        std::cout << "\n";

    }

};

