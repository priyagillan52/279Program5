//graph .h file
//Author: Priya Gillan

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <queue>

using namespace std;

class Graph{
    private:

        //struct used as vertex in the graph
        struct Node{
            int endPoint;
            int weight;
        };

        //this struct is used in the priority queue
        struct pqElement{
            int start;
            int end;
            int weight;
        };

        //operator operloader (greater than) for priority queue 
        struct greater{
            bool operator()(pqElement const& a, pqElement const& b)const{
                return a.weight < b.weight;
            }
        };

        //operator operloader (less than) for priority queue 
        struct less{
            bool operator()(pqElement const& a, pqElement const& b)const{
                return a.weight > b.weight;
            }
        };

    public:
        //adjacency list for the graph
        vector<list<Node>> graph;

        //keeps track of the vertex names
        vector<string> vertices;

        //number of vertices
        int numVertices;

        //number of edges
        int numEdges;

    public:
        //constructor for the graph
        Graph();

        //reads in the graph from file and will return if successful
        bool readGraph(const string &inFileName);

        //prints out the graph
        void printGraph();

        //computes topological sort
        //precondition: vertex must be in graph
        void computeTopologicalSort();

        //computes the shortest path
        void computeShortestPaths(string source);

        //computes the minimum spanning tree
        //predondition: graph must be connected 
        void computeMinimumSpanningTree();
    private:
        //helper method that prints the shortest path 
        void printShortest(vector<int> prev, int destination);

        //helper method that find the index that a vertex is at 
        int findIndex(string index);

        //helper method that adds vertex to graph
        void addToGraph(int &start, int &end, int &weight);
};

#endif