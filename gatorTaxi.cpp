#include <fstream>        //initializing filestream and vector 
#include<vector>
#include "MyRBT.h"        //including files MyRBT.h and stringUtils.h 
#include "stringUtils.h"

void print(RBT *rbt, Heap *heap, int *args, string &output) {       //function to print output 
cout << "print: " << args[0] << endl;
output += rbt -> range(args[0], args[0]);
}

void printRange(RBT *rbt, Heap *heap, int *args, string &output) {      //function to print range of output
    cout << "printRange: " << args[0] << " " << args[1] << endl;
    output += rbt -> range(args[0], args[1]);
}

void insertRide(RBT *rbt, Heap *heap, int* args, string &output) {             //this function is used to insert the ride using node RBTNode and HeapNode
    cout << "insert: " << args[0] << " " << args[1] << " " << args[2] << endl;
    RBTNode* rnode = new RBTNode(nullptr, nullptr, nullptr, nullptr, RED, args[0], args[1], args[2]);
    HeapNode* hnode = new HeapNode(nullptr, args[0], args[1], args[2]);
rnode -> heapNode = hnode;
   hnode -> rbtNode = rnode;

    if(rbt -> findNode(rnode -> rideNumber, rbt -> root)) {      //check if a node with the same ride number as the new node already exists in the RBT. If such a node is found, it appends the string "Duplicate RideNumber\n" to the output string and returns immediately from the function.
        output += "Duplicate RideNumber\n";
        return;
    }

    rbt -> insert(rnode);   //insert
    heap -> push(hnode);    //push operation

    rbt -> printTree();   //print tree
    heap -> printHeap();  //print heap
}

void getNextRide(RBT *rbt, Heap *heap, string &output) {         //function that takes a pointer to an RBT object, a pointer to a Heap object, and a reference to a string output as arguments and returns nothing (void)
    cout << "getNextRide" << endl;

    if(heap -> getSize() == 0) {
        output += "No active ride requests\n";
        return;
    }
    HeapNode* deletedHeapNode = heap -> pop();                //removes the node with the minimum ride cost or trip duration from the heap by calling the pop function of the heap object. The removed node is then printed to the output string by calling its printHeapNode function. The corresponding node in the RBT is then deleted by calling the deleteNode function of the rbt object with the ride number of the removed heap node as an argument. Finally, the heap and RBT are printed to the standard output stream by calling their respective printHeap and printTree functions.
    deletedHeapNode -> printHeapNode(output);                           
    rbt -> deleteNode(deletedHeapNode -> rbtNode -> rideNumber);

    cout << endl;
    heap -> printHeap();
    rbt -> printTree();
}

void cancelRide(RBT *rbt, Heap *heap, int* args, string &output) {   //function cancels a ride by removing the corresponding node from both the RBT and the heap. 
    cout << "cancelRide: " << args[0] << endl;
    RBTNode* deletedRBTNode = rbt -> deleteNode(args[0]);
    heap -> remove(deletedRBTNode -> heapNode);   //calls the remove function of the heap object with the value of the heapNode member variable of the deleted RBT node as an argument. The function removes the corresponding node from the heap.

    heap -> printHeap();
    rbt -> printTree();
}

void updateTrip(RBT *rbt, Heap *heap, int* args, string &output) {
    int newTD = args[1];
    cout<< "updateTrip: " << args[0] << " " << newTD << endl;
    RBTNode* node = rbt -> findNode(args[0], rbt -> root);
    if(node) {
        int existingTD = node -> tripDuration;
        if(newTD <= existingTD) {
            node -> tripDuration = newTD;
            heap -> update(node -> heapNode , node -> rideCost, newTD);
            heap -> printHeap();
            rbt -> printTree();
        } else if(existingTD < newTD && newTD <= 2 * existingTD) {
            cancelRide(rbt, heap, args, output);
            args[1] = node -> rideCost + 10;
            args[2] = newTD;
            insertRide(rbt, heap, args, output);
        } else if(newTD > 2 * existingTD) {
            cancelRide(rbt, heap, args, output);
        }
    } else cout << "No trip to update" << endl;
}

void parseInput(RBT *rbt, Heap *heap, int argc, char **argv) {
    fstream inputFile;
    inputFile.open(argv[1], ios::in);
    string output;

    if(inputFile.is_open()){
        string fileLine;
        while(getline(inputFile, fileLine)){ 
            string operationPrefix = fileLine.substr(0, 3);
            processOperation(rbt, heap, fileLine, operationPrefix, output);
        }
        inputFile.close();
    }

    ofstream MyFile("output_file.txt");
    MyFile << output;
    MyFile.close();
}

int main(int argc, char **argv) {
    RBT *rbt = new RBT();
    Heap *heap = new Heap();

    if(argc >= 2) {
        parseInput(rbt, heap, argc, argv);
        return 0;
    } else cout << "Insufficient arguments - Please provide input file" << endl;
    return 0;

    RBTNode *rnode;
    HeapNode *hnode;
    heap -> push(new HeapNode(nullptr, 10,11,12));
    heap -> push(new HeapNode(nullptr, 20,9,12));
    heap -> push(new HeapNode(nullptr, 30,9,9));
    
    heap -> push(new HeapNode(nullptr, 20,8,12));
    HeapNode *h = new HeapNode(nullptr, 30,9,8);
    heap -> push(h);
    heap -> printHeap();heap -> remove(h);
    heap -> printHeap();

}


/
