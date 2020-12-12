//graph implementation
//Author: Priya Gillan

#include "Graph.h"
#include "DisjointSet.h"
#include <algorithm>
#include <fstream>
#include <sstream>

//graph construct that initializes the number of edges and the number of vertices to be 0
Graph::Graph(){ 
    numEdges = 0; 
    numVertices = 0; 

}

//function that reads in the graph.
bool Graph::readGraph(const string &inFileName){

    //opens the file to be read
    ifstream infile(inFileName); 

    //if the file cannot be open then return false
    if (infile.fail())
    {
        return false;
    }

    //clears graph vector and vector with vertex names if a graph has 
    //already been read in
    if(numVertices != 0){
        graph.clear();
        vertices.clear();
        numVertices = 0;
        numEdges = 0;
    }

    //intialize next element string
    string nextElement;

    //get next item and assign to number of vertices
    infile >> (numVertices);

    //read in each vertex name and add the name to a vector
    for(int i = 0; i < numVertices; i++){
        infile >> nextElement;
        vertices.push_back(nextElement);
        list<Node> tempList;

        //put an empty list at each vertex of the graph
        graph.insert(graph.begin()+i, tempList);
    }

    //get next item from graph and assign to num edges
    infile>>numEdges;

    //add the edges to the graph
    for(int i = 0; i < numEdges; i++){
        infile>>nextElement;
        string startV = nextElement;

        infile>>nextElement;
        string endV = nextElement;

        infile>>nextElement;
        istringstream iss3 (nextElement);
        int vertexW;
        iss3>>vertexW;

        int start = findIndex(startV);

        int end = findIndex(endV);

        addToGraph(start, end, vertexW);
    }
    return true;
}

//this method prints out the graph
void Graph:: printGraph(){
    cout<<numVertices<<endl;
    for(int i = 0; i<numVertices; i++){
        cout<<vertices.at(i)<<endl;
    }
    cout<<numEdges<<endl;
    for(int i = 0; i<numVertices; i++){
        list<Node> &tempList = graph.at(i);
        for(list<Node>::iterator it = tempList.begin(); it != tempList.end(); ++it){
            Node &temp = *it;
            cout<<vertices.at(i)<<" ";
            string endVal = vertices.at(temp.endPoint);  
            cout<<endVal<<" "; 
            cout<<temp.weight<<endl;
        }
    }

}

//this method finds the index that a string is at
int Graph::findIndex(string input){
    for(int i = 0; i < vertices.size(); i++){
        if(input == vertices[i]){
            return i;
        }  
    }
}
    

//a helper method that adds items to the graph
void Graph::addToGraph(int &start, int &end, int &weight){
    
    Node newNode;
    newNode.endPoint = end;
    newNode.weight = weight;

    graph[start].push_back(newNode);
}

//method that computes the topological sort
void Graph::computeTopologicalSort(){

    //vector that keeps track of in degree of each vertex
    vector<int> inDegree;

    //keeps track of the order
    vector<int> topOrder;

    //queue to handle search
    queue<int>BFS;

    //keeps track of number of vertices visited
    int count = 0;

    //creates a space for each vertex of the graph
    for(int i = 0; i<numVertices; i++){
        inDegree.push_back(0);
    }

    //goes through the graph to figure out the indegree of each vertex
    for(int i = 0; i<numVertices; i++){
        list<Node> &tempList = graph.at(i);
        for(list<Node>::iterator it = tempList.begin(); it != tempList.end(); ++it){
            Node &temp = *it;
            int endNode = temp.endPoint;
            
            int value = inDegree[endNode];
            
            inDegree[endNode] = value+=1;
        }      
    }
    
    //put all the vertices in the queue
    for(int i = 0; i<numVertices; i++){
           
        if(inDegree[i] == 0){
                
            BFS.push(i);
        }
    }

        //while the queue isn't empty
        while(!BFS.empty()){

        //pop off the front node
        int node = BFS.front();
        BFS.pop();
        topOrder.push_back(node);

        //go though the graph and find the next connected vertices
        list<Node>::iterator it;
        for(it = graph[node].begin(); it != graph[node].end(); it++){
            Node &temp = *it;
            int endNode = temp.endPoint;
            if(--inDegree[endNode] == 0){
                //add to the queue
                BFS.push(endNode);
            }
        }
        count++;
    }

    //if the num visited doesn't num of vertices then the graph can't be topologically sorted
    if(count != numVertices){
        cout<<"This graph cannot be topologically sorted."<<endl;
        return;
    }

    //prints out the topological sort
    cout<<"Topological Sort:"<<endl;
    for(int i = 0; i< topOrder.size(); i++){
        int nodeToPrint = topOrder[i];
        if(i < topOrder.size()-1){
            cout<<vertices[nodeToPrint] <<" --> ";
        }
        else{
            cout<<vertices[nodeToPrint];
        }
    }
    cout<<endl;
    
}

//method that computes the shortest path using Dijkstra's Algorithm
//Precondition: vertex must be in the graph
void Graph::computeShortestPaths(string source){

    //find the index of the vertex vector
    int sourceNode = findIndex(source);

    //vector to keep track of the parent vertices
    vector<int> prev(numVertices, -1);

    //vector that keeps track of the distances and initialize everything to -1
    vector<int> distanceVec(numVertices);

    vector<bool> found(numVertices, false);

    //create a priority queue of priority queue elements
    priority_queue<pqElement, vector<pqElement>, greater> pq;

    distanceVec[sourceNode] = 0;
    found[sourceNode] = true;

    pqElement temp;
    temp.end = sourceNode;
    temp.weight = distanceVec[sourceNode];
    pq.push(temp);
    
    while(!pq.empty()){
        //pop off the top element
        pqElement temppq = pq.top();

        //starting vertex is the detination of the previous element
        int startVertex = temppq.end;
        
        pq.pop();

        for(int i = 0; i<numVertices; i++){
            //goes through graph at start vertex
            list<Node>::iterator it;

            //find adjacent vertex
            for(it = graph[startVertex].begin(); it != graph[startVertex].end(); it++){
                
                //creates a temp node to find all adjacent vertices to the start vertex
                Node &temp = *it;
                int adjVertex = temp.endPoint;
                int adjWeight = temp.weight;

                //calculates to see where the weight is greater
                if(!found[adjVertex] || distanceVec[adjVertex]>distanceVec[startVertex] + adjWeight){

                    //updates the distace vector at the adjacent vertex
                    distanceVec[adjVertex] = distanceVec[startVertex] + adjWeight;

                    //creates new priority queue element for the adjacent vertex
                    pqElement newElement;
                    newElement.weight = distanceVec[adjVertex];
                    newElement.end = adjVertex;
                    newElement.start = startVertex;

                    //finds the parent vertex
                    prev[adjVertex] = startVertex;
                    
                    //update the vertex so it is found
                    found[adjVertex] = true;

                    //push new vertex to priority queue
                    pq.push(newElement);
                }
            }

        }
 
    }
    //prints the shortest paths
    cout<<"Shortest paths from "<<source<<":"<<endl;
    for(int i = 0; i<numVertices; i++){
        
        if(!(found[i])){
            cout<<"No path from "<<source<<" to "<<vertices[i]<<" found."<<endl;
        }
        else if(distanceVec[i] > 0){
            cout<<source;
            printShortest(prev, i);
            cout<<" || Weight: "<<distanceVec[i]<<endl;
        }
    }
}

//helper method that uses recusion to find the shortest path for printing
void Graph::printShortest(vector<int> parent, int destination){

    if(parent[destination] == -1){
        return;
    }
    
    else{
        printShortest(parent, parent[destination]);
        cout<<" --> ";
        cout<<vertices[destination];
    }
    
}

//method that computes the minimum spanning tree
//precondition: graph must be connected
void Graph::computeMinimumSpanningTree(){

    //create a new disjoint set with the number of vertices
    DisjointSet mySet(numVertices);
    
    //create a vector that keeps track of the minimum tree
    vector<pqElement> minTree;

    //create priority queue that will order everything from least to greatest
    priority_queue<pqElement, vector<pqElement>, less> pq;

    //go through the graph and convert the vertices to priority queue elements so 
    //they can be added and sorted
    for(int i = 0; i<numVertices; i++){
        list<Node>::iterator it;
        for(it = graph[i].begin(); it != graph[i].end(); it++){ 
            Node &temp = *it;
            pqElement newElement;
            newElement.weight = temp.weight;
            newElement.end = temp.endPoint;
            newElement.start = i;
            pq.push(newElement);
        }
    }

    //holds the value for the cost of the tree
    int cost;

    //keeps track of the number of vertices visited
    int numVisited = 0;

    while(numVisited < numVertices-1){
        
        //pop off the top element
        pqElement nextElement = pq.top();
        pq.pop();
        
        //find the root of each vertex
        int set1 = mySet.find(nextElement.start);
        int set2 = mySet.find(nextElement.end);

        //if they are not in the same set then union them
        if(set1 != set2){
            minTree.push_back(nextElement);

            //update the number visited
            numVisited++;

            mySet.doUnion(set1, set2);

            //update the cost
            cost+=nextElement.weight;
        }
    }

    //print out the minimum spanning tree
    cout<<"Minimum Spanning Tree:"<<endl;
    for(int i = 0; i<minTree.size(); i++){
        pqElement &temp = minTree[i];
        cout<<vertices[temp.start]<<" -- "<<vertices[temp.end]<<" || Weight: "<<temp.weight<<endl;
    }
    cout<<"Total Cost: "<<cost<<endl;

}

