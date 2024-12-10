/******************************************************************************
     Name:           Ariella Marchuk
     Email:          amarchuk@pdx.edu
     Date:           December 10th 2024
     Filename:       Marchuk_Ariella_Program5.h
     Class:          CS163 Section 002 Online
     File Description:
                 Header file for Graph class.
                 Task 1: build adjacency list
                 Task 2: insert a vertex (a geocache) 
                 Task 3: insert an edge   
                 Task 4: display adjacency list                                  
                 Task 5: display closest geocache    
                 Task 6: (DESTRUCTOR) destroy all dynamic memory
                 Task 7: breadth first algorithm using recursion
                            Example:      GRAPH
                                        1   -->    3 
                                    / 
                                0       |          |
                                    \
                                        2   -->    4
                     - can be two arrays: visited and queue. both empty
                     - push 0 onto queue and mark it visited with 0
                     - remove node 0 from front. visit unvisited. push them onto queue
                     - remove node 1 from front. visit unvisited. push them onto queue
                     - remove node 2 from front. visit unvisited. push them onto queue
                     - remove node 3 from front. visit unvisited. push them onto queue
                     - remove node 4 from front. visit unvisited. push them onto queue
******************************************************************************/
#ifndef MARCHUK_ARIELLA_PROGRAM5_H
#define MARCHUK_ARIELLA_PROGRAM5_H
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct edgeNode{
    int index;          // index to connecting vertex
    double distance;    // how far to the next one in miles

    edgeNode* next;     // ptr to another edge node

};

struct adjList{ 
    string coords;              // cache coordinates
    string location_descrip;    // descripton of cache location
    string hint;                // hint for how to solve cache
    
    edgeNode* pointer; 
};


// array of vertices where each index has a head pointer to a LLL of edge nodes
class adjGraph{
    public:
        adjGraph();
        ~adjGraph();
      
        // allocate memory for adjList* vertices based on vertex_count
        // initialize coords, location_descrip, and ptr for each vertex
        bool build_adjlist(int vertex_count, const vector<tuple<string, string, string>>& vertex_data);
 
     
        // access start vertex in vertices[], dynamically create new edgeNode,
        // set index = end and distance = distance, insert new edgeNode into ptr of start
        bool insert_edge(int start, int end, double distance);              // add edge: 
                                                                            // [i] start vertex, 
                                                                            // [i] end vertex, 
                                                                            // [d] distance 
        
        // dynamically create new adjList entry, populate fields, add to next slot in vertices[]
        bool insert_vertex(string coords, string descrip, string hint);     // add vertex: 
                                                                            // [s] coords, 
                                                                            // [s] descrip, 
                                                                            // [s] hint

        // iterate through vertices[], for each vertex travers LL of edgeNode using ptr 
        bool display_adjlist() const;   // display all vertices and their connected edges
        
        // use BFS from starting vertex to find nearest unvisited cache & display result
        bool display_nearest_cache(int start, int& nearest, double& min_distance);   // find & display closest geocache 
      
        // use queue to explore reachable vertexes starting from source, mark as visited
        // can help with finding specific data (nearest cache)
        bool breadthFirst(int start, vector<int>& marathon_order) const; 
        
        // return vertex count
        int get_vertex_count() const;

        // return vertices for read only
        const vector<adjList>& get_vertices() const;

        void find_nearest(int current, double& min_distance, int& nearest);  // helper
        
        void bfs(int current, queue<int>& traversal_queue, 
                              vector<bool>& visited, 
                              vector<int>& marathon_order) const; // helper

    private: 
        vector<adjList> vertices;
        vector<bool> visited;
        int vertex_count;
        
        void display_edges(edgeNode* current) const;   // helper 
        void traverse_edges(edgeNode* edge, double& min_distance, int& nearest); // helper
        void bfs_edges(edgeNode* edge, queue<int>& traversal_queue, vector<bool>& visited) const; // helper
};
#endif //MARCHUKARIELLAPROGRAM5H
