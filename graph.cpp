/****************************************************************************** 
     Name:           Ariella Marchuk 
     Email:          amarchuk@pdx.edu
     Date:           December 10th 2024
     Filename:       Marchuk_Ariella_Program5.cpp
     Class:          CS163 Section 002 Online
     File Description:
                implementation of Graph class
                Task 1: build adjacency list
                Task 2: insert a vertex (a geocache)
                Task 3: insert an edge (how to get to next geocache
                                        i.e. inserting a node into the
                                        edge list. has data for distance
                                        to next geocache aka weighted graph)
                Task 4: display adjacency list (display what geocache are
                                        available and not visited)
                Task 5: display closest geocache (specify location geocache,
                                        find all available geocache
                                        to visit next and display them)
                Task 6: (DESTRUCTOR) destroy all dynamic memory
                Task 7: breadth first algorithm using recursion
******************************************************************************/
#include <iostream>
#include <string>
#include <limits>
#include "graph.h"
using namespace std;

/***** TASK 1 : class CONSTRUCTOR *****/
adjGraph::adjGraph()
{
    vertex_count = 0;   // always start with 0 vertices
    visited.clear();    // clearing visited flags
}

/***** TASK 6 : class DESTRUCTOR *****/
adjGraph::~adjGraph()
{
    // deallocate all edgeNodes
    for(auto& vertex : vertices) // auto for deducing type by reference
    {
        edgeNode* current = vertex.pointer; // point to head of adjlist
        while (current) 
        {
            edgeNode* temp = current;
            current = current->next;
            delete temp;
        }
        vertex.pointer = nullptr;
    }
    vertices.clear();
}

//***** TASK 1 : build_adjlist *****/
bool adjGraph::build_adjlist(int vertex_count, const vector<tuple<string, string, string>>& vertex_data)
{
    this->vertex_count = vertex_count;  // set vector count 
    vertices.resize(vertex_count);      // resize vector to count 

    // initialize each vertex 
    for (int i = 0; i < vertex_count; ++i)
    {
        vertices[i].coords = get<0>(vertex_data[i]);            // gps coord 
        vertices[i].location_descrip = get<1>(vertex_data[i]);  // location decsrip 
        vertices[i].hint = get<2>(vertex_data[i]);              // hint
        vertices[i].pointer = nullptr;                          // no edges yet 
    }

    return true; // build adjlist SUCCESS 
}
//***** TASK 2 : insert vertex *****/
bool adjGraph::insert_vertex(string coords, string descrip, string hint)
{ 
    adjList new_vertex;

    new_vertex.coords = coords;
    new_vertex.location_descrip = descrip;
    new_vertex.hint = hint;

    new_vertex.pointer = nullptr; 

    vertices.push_back(new_vertex); // add new vertex to vertices vector
 
    vertex_count++; // increment count for vertex

    return true; // vertex added SUCCESS 
}

//***** TASK 3 : insert edge *****/
bool adjGraph::insert_edge(int start, int end, double distance)
{
    if (start < 0 || start >= vertex_count || end < 0 || end >= vertex_count)
    {
        cout << "invalid vertex index. edge add FAILED.\n";
        return false;
    }

    // check for reverse edge (end -> start) 
    // to help calculations for nearest cache....
    // if index 0 - 1 has one distance, and user makes index 1 - 0 have a diff
    // distance, this is for edge validation. avoids redundant checks/visits
    // considers one valid connection
    edgeNode* current = vertices[start].pointer;
    while(current)
    {
        if(current->index == end)
        {
            if(current->distance == distance)
            {
                return false; // duplicate edge found
            }
            else
            {
                current->distance = distance; // update distance if different?
                break; 
            }
        }
        current = current->next;
    }

    // if edge doesnt exist, add it 
    if(!current) 
    {
        edgeNode* new_edge = new edgeNode;
        new_edge->index = end;
        new_edge->distance = distance;
        new_edge->next = vertices[start].pointer;
        vertices[start].pointer = new_edge;
    }
    
    // repeat for reverse edge...
    current = vertices[end].pointer;
    while(current)
    {
        if(current->index == start)
        {
            current->distance = distance;
            return true;
        }
        current = current->next;
    }
   
    // im adding bidirectional edges because in reality, if you walk somewhere, 
    // you can walk backwards the same distance. visited flags
    // will keep this at bay for BFS traversal.....
    edgeNode* reverse_edge = new edgeNode;
    reverse_edge->index = start;
    reverse_edge->distance = distance;
    reverse_edge->next = vertices[end].pointer;
    vertices[end].pointer = reverse_edge;
    

    return true; // edge added SUCCESS
}

//***** TASK 4 : display adjlist *****/
      
bool adjGraph::display_adjlist() const
{
    if (vertex_count == 0) // vertices? 
    {
        return false; // graph empty
    }

    for (int i = 0; i < vertex_count; ++i)
    {
        cout << "[" << i << "] " << vertices[i].coords << " - " << vertices[i].location_descrip
             << " (hint: " << vertices[i].hint << ")\n";

        if (!vertices[i].pointer) // vertex has edges? 
        {
            cout << "|  -> no connections\n";
        }
        else
        {
            cout << "|  -> connections:\n";
            display_edges(vertices[i].pointer); // helper call 
        }
        cout << "|\n";  
    }

    return true; // display SUCCESS 
}

// recursive helper for edge traversal
void adjGraph::display_edges(edgeNode* current) const
{
    if (!current) // base case
    {
        return;
    }

    cout << "|     -> [" << current->index << "] " << vertices[current->index].coords
         << " (" << current->distance << " miles away)\n";
    display_edges(current->next); // recurse to next edge
}
        
//***** TASK 5 : display closest geocache *****/
/* 1. input starting index 2. error check if value is valid 
   3. reset visited flag 4. find_nearest */

bool adjGraph::display_nearest_cache(int start, int& nearest, double& min_distance)
{
    if (start < 0 || start >= vertex_count){return false;}
    visited.assign(vertex_count, false); 
    
    // nearest cache
    nearest = -1;                        
    min_distance = numeric_limits<double>::max(); 
    find_nearest(start, min_distance, nearest);
    return nearest != -1; // SUCCESS nearest cache was found
}

// HELPER traversal for vertices
void adjGraph::find_nearest(int current, double& min_distance, int& nearest)
{
    visited[current] = true; // mark current vertex as visited 
    traverse_edges(vertices[current].pointer, min_distance, nearest);
}

// HELPER traversal for edges
void adjGraph::traverse_edges(edgeNode* edge, double& min_distance, int& nearest)
{
    if (!edge){return;} 

    if (!visited[edge->index]) // unvisited nodes
    {
        if (edge->distance < min_distance)
        {
            min_distance = edge->distance;
            nearest = edge->index;
        }
        // connected vertex
        find_nearest(edge->index, min_distance, nearest);
    }
    // to next edge
    traverse_edges(edge->next, min_distance, nearest);
}
        
// HELPER for main case 7; task 5
void handle_display_nearest_cache(adjGraph* graph)
{
    if (!graph)
    {
        cout << "initialize graph first (Option 1).\n";
        return;
    }
    if (graph->get_vertex_count() == 0)
    {
        cout << "no vertices available. please add vertices first (Option 4).\n";
        return;
    } 
    
    cout << "\ncurrent geocaches:\n";
    const vector<adjList>& vertices = graph->get_vertices();
    for (int i = 0; i < graph->get_vertex_count(); ++i)
    {
        cout << "|  [" << i << "] " << vertices[i].coords << " - " << vertices[i].location_descrip << "\n";
    } 

    int start;
    cout << "enter the starting geocache index: ";
    cin >> start;

    int nearest = -1;
    double min_distance = -1;

    // find and display the nearest cache
    if (graph->display_nearest_cache(start, nearest, min_distance))
    {
        cout << "nearest unvisited cache: [" << nearest << "] "
             << graph->get_vertices()[nearest].coords << " - "
             << graph->get_vertices()[nearest].location_descrip
             << " (" << min_distance << " miles away)\n";
    }
    else
    {
        cout << "unable to find nearest cache or all caches visited.\n";
    }
}
 
//***** TASK 7 : breath first algorithm *****/
/*  breadthFirst    :    bfs   :   bfs_edges*/

bool adjGraph::breadthFirst(int start, vector<int>& marathon_order) const
{
    if (start < 0 || start >= vertex_count){return false;} // check for valid index

    vector<bool> visited(vertex_count, false); // reset visited flags
    queue<int> traversal_queue; // will be used to store vertices checked during BFS

    traversal_queue.push(start);  // start with given geocache
    visited[start] = true;        // mark visited
    marathon_order.clear();       // clear marathon order
 
    bfs(traversal_queue.front(), traversal_queue, visited, marathon_order); // call bfs logic helper

    return true; // SUCCESS BFS completed
}

// HELPER for BFS ; main logic
void adjGraph::bfs(int current, queue<int>& traversal_queue, vector<bool>& visited, vector<int>& marathon_order) const
{ 
    
    // if traversal_queue empty, no more vertices to process; empty queue 
    if (traversal_queue.empty()){return;}   
 
    traversal_queue.pop();              //dequeue current geocache
    marathon_order.push_back(current);  //push back and process geocache
    
    // go through all unvisited edges connected to current vertex
    bfs_edges(vertices[current].pointer, traversal_queue, visited); // go through all unvisited edges

    if (!traversal_queue.empty()) // move on algorithm to next geocache in queue
    {
        bfs(traversal_queue.front(), traversal_queue, visited, marathon_order);
    }
}

// HELPER for BFS: helps with traversing edges used in algorithm
void adjGraph::bfs_edges(edgeNode* edge, queue<int>& traversal_queue, vector<bool>& visited) const
{ 
    if (!edge){return;} // base case, no edges to check
 
    if (!visited[edge->index]) // if vertex is unvisited
    {
        traversal_queue.push(edge->index); // push index to traversal queue
        visited[edge->index] = true;       // mark it as visited
    }
 
    bfs_edges(edge->next, traversal_queue, visited); // move to next edge
}

// HELPER for BFS in MAIN task 7 ; case 8
void handle_breadth_first(adjGraph* graph)
{
    // ERROR handling 
    if (!graph)
    {
        cout << "initialize graph first (option 1).\n";
        return;
    }
    if (graph->get_vertex_count() == 0)
    {
        cout << "no vertices available. please add vertices first (option 4).\n";
        return;
    }
    
    // DISPLAY current geocaches with index values
    cout << "\ncurrent geocaches:\n";

    const vector<adjList>& vertices = graph->get_vertices();

    for (int i = 0; i < graph->get_vertex_count(); ++i)
    {
        cout << "|  [" << i << "] " << vertices[i].coords << " - " << vertices[i].location_descrip << "\n";
    }
    
    // INPUT to start algorithm
    int start;
    cout << "\nenter the starting geocache index for BFS traversal: ";
    cin >> start;
    
    // BEGIN BFS
    vector<int> marathon_order;
    if (graph->breadthFirst(start, marathon_order))
    {
        cout << "\nBFS geocache marathon order:\n";
        for (int index : marathon_order)
        {
            cout << "|  [" << index << "] " 
                 << vertices[index].coords 
                 << " - " 
                 << vertices[index].location_descrip 
                 << " [VISITED]\n";
        }

        // DISPLAY path  
        cout << "\ntraversal path:\n";
        // avoiding overflow with negative indexes
        for (size_t i = 0; i < marathon_order.size(); ++i)
        {
            if (i > 0)
                cout << " -> ";
            cout << "[" << marathon_order[i] << "]";
        }
        cout << "\n";

        // DISPLAYING distances 
        cout << "\nBFS marathon order with distances:\n";
        for (int index : marathon_order)
        {
            cout << "|  [" << index << "] " 
                 << vertices[index].coords 
                 << " - " 
                 << vertices[index].location_descrip;

            if (vertices[index].pointer)
            {
                edgeNode* current = vertices[index].pointer;
                cout << " (distances: ";
                while (current)
                {
                    cout << current->distance 
                         << " miles to [" 
                         << current->index 
                         << "]";
                    if (current->next)
                        cout << ", ";
                    current = current->next;
                }
                cout << ")";
            }
            cout << "\n";
        }     
        cout << "geocache BFS algorithm SUCCESS.\n";
    }
    else
    {
        cout << "invalid starting geocache index.\n";
    }
}

//***** CLASS HELPERS FOR PRIVATE *****/
int adjGraph::get_vertex_count() const{return vertex_count;}
const vector<adjList>& adjGraph::get_vertices() const{return vertices;}

//***** MAIN HELPER *****/
void display_menu()
{
    cout << "\n|||||||||||||||||||||||||||| geocache graph menu |||||||||||||||||||||||||||||||||\n";
    cout << "1. test constructor        2. test Destructor        3. build adjacency list\n";
    cout << "4. insert a vertex         5. insert an Edge         6. display adjacency list\n";
    cout << "7. display nearest cache   8. test breadth-first     9. exit\n";
    cout << "menu choice: ";
}

// begin main    
int main()
{
    adjGraph* graph = nullptr;                         // ptr to dynamically control graph 
    vector<tuple<string, string, string>> vertex_data; // hold vertex info
    int choice = 0;

    do
    {
        display_menu();
        cin >> choice;

        switch (choice)
        {
        case 1: // CONSTRUCTOR
        if (graph)
            {
                cout << "graph already initialized. DESTROY FIRST.\n";
            }
            else
            {
                graph = new adjGraph(); 
                cout << "graph initialized SUCCESS\n";
            }
            break;

        case 2: // DESTRUCTOR 
            if (!graph)
            {
                cout << "no graph to destroy.\n";
            }
            else
            {
                delete graph; 
                graph = nullptr;
                cout << "graph destroyed SUCCESS.\n";
            }
            break;

        case 3: // BUILD ADJACENCY LIST 
        if (!graph)
            {
                cout << "please initialize graph first (Option 1).\n";
            }
            else
            {
                int num_vertices;
                cout << "number of vertices? : ";
                cin >> num_vertices;
                vertex_data.clear();

                for (int i = 0; i < num_vertices; ++i)
                {
                    string coords, desc, hint;
                    cout << "\ninsert vertex details [" << i + 1 << "]:\n";
                    cout << "|  coordinates: ";
                    cin >> coords;
                    cin.ignore();
                    cout << "|  description: ";
                    getline(cin, desc);
                    cout << "|  hint: ";
                    getline(cin, hint);
                    vertex_data.push_back({coords, desc, hint});
                }

                if (graph->build_adjlist(vertex_data.size(), vertex_data))
                {
                    cout << "adjacency list built SUCCESS.\n";
                }
                else
                {
                    cout << "FAILED to build adjacency list.\n";
                }
            }
            break;

        case 4: // INSERT VERTEX 
            if (!graph)
            {
                cout << "initialize graph first (Option 1).\n";
            }
            else
            {
                string coords, desc, hint;
                cout << "enter new vertex details:\n";
                cout << "|  coordinates: ";
                cin >> coords;
                cin.ignore();
                cout << "|  description: ";
                getline(cin, desc);
                cout << "|  hint: ";
                getline(cin, hint);

                if (graph->insert_vertex(coords, desc, hint))
                {
                    cout << "vertex added SUCCESS.\n";
                }
                else
                {
                    cout << "adding vertex FAILED.\n";
                }
            }
            break;  

        case 5: // INSERT EDGE 
        
            if (!graph)
            {
                cout << "initialize graph first (option 1).\n";
            }
            else
            {
                if (graph->get_vertex_count() == 0)
                {
                    cout << "no vertices available. please add vertices first (option 4).\n";
                }
                else
                {
                    cout << "\ncurrent geocaches:\n";

                    const vector<adjList>& vertices = graph->get_vertices();

                    for (int i = 0; i < graph->get_vertex_count(); ++i)
                    {
                        cout << "|  [" << i << "] " << vertices[i].coords << " - " << vertices[i].location_descrip << "\n";
                    }

                    int start, end;
                    double distance = -1;

                    cout << "\nconnect geocaches by entering their index:\n";
                    cout << "|  starting geocache index: ";
                    cin >> start;
                    cout << "|  connecting geocache index: ";
                    cin >> end;

                    if (start == end)
                    {
                        cout << "cannot create an edge to the same geocache.\n";
                        break;
                    }

                    while (true)
                    {
                        cout << "|  how far away is connecting index in miles?: ";
                        cin >> distance;

                        if (!cin.fail() && distance > 0)
                        {
                            break;
                        }
                        else
                        {
                            cout << "enter a positive distance value in miles.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }

            
                    if (graph->insert_edge(start, end, distance))
                    {
                        cout << "edge added SUCCESS.\n";
                    }
                    else
                    {
                        cout << "adding edge FAILED. either invalid indices or conflicting distance with reverse edge.\n";
                    }
                }
            }
            break;

        case 6: // DISPLAY ADJACENCY LIST 
            if (!graph)
            {
                cout << "initialize graph first (Option 1).\n";
            }
            else
            {
                cout << "\ndisplaying adjacency list...\n";
                if (!graph->display_adjlist())
                {
                    cout << "no geocaches in the graph to display.\n";
                }
                else
                {
                    cout << "\nadjacency list displayed SUCCESS.\n";
                }
            }
            break;

        case 7: // DISPLAY NEAREST CACHE 
            cout << "\n Check the nearest geocache from any current one.\n";
            handle_display_nearest_cache(graph); 
            break;

        case 8: // BREADTH FIRST SEARCH  
            cout << "\nPlan a geocaching marathon! Enter your starting location and see nearby paths.\n";
            cout << "\n Handling our Breadth First Algorithm...\n";
            handle_breadth_first(graph);
         
            break;

        case 9: // exit
            cout << "\n                                   exiting geocacher  . \n";
            cout << "                                initiating cleanup    .\n\n\n";
            if (graph)
            {
                delete graph; 
                graph = nullptr;
            }
            break;

        default:
            cout << "\nyou have to choose a number between 1 - 9\n";
        }
    } while (choice != 9);

    return 0;
}
// end main
