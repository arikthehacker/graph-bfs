/****************************************************************************** 
     Name:           Ariella Marchuk 
     Email:          amarchuk@pdx.edu
     Date:           December 6th 2024
     Filename:       Marchuk_Ariella_Program5.cpp
     Class:          CS163 Section 002 Online
     File Description:

                implementation of Graph class

         done   Task 1: build adjacency list

         done   Task 2: insert a vertex (a geocache)

         done   Task 3: insert an edge (how to get to next geocache
                                        i.e. inserting a node into the
                                        edge list. has data for distance
                                        to next geocache aka weighted graph)

         done   Task 4: display adjacency list (display what geocache are
                                                available and not visited)

                Task 5: display closest geocache (specify location geocache,
                                                  find all available geocache
                                                  to visit next and display them)

                Task 6: (DESTRUCTOR) destroy all dynamic memory

                Task 7: breadth first algorithm using recursion

        TODO:
        * refactor insert edge so its more modular
        * break down insert edge and distance validation into helper functions
        * refactor nesting in main
        * simplify display function


******************************************************************************/
#include <iostream>
#include <string>
#include <limits>
#include "Marchuk_Ariella_Program5.h"
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

    return true; // successful 
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
            display_edges_recursive(vertices[i].pointer); // helper call 
        }
        cout << "|\n";  
    }

    return true; // display SUCCESS 
}

// recursive helper for edge traversal
void adjGraph::display_edges_recursive(edgeNode* current) const
{
    if (!current) // base case
    {
        return;
    }

    cout << "|     -> [" << current->index << "] " << vertices[current->index].coords
         << " (" << current->distance << " miles away)\n";
    display_edges_recursive(current->next); // recurse to next edge
}
        
//***** TASK 5 : display closest geocache *****/
//***** TASK 7 : breath first algorithm *****/
//***** CLASS HELPERS *****/
int adjGraph::get_vertex_count() const
{
    return vertex_count;
}

const vector<adjList>& adjGraph::get_vertices() const
{
    return vertices;
}

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

        case 7: // display nearest cache 
            cout << "\n[SORRY] nearest cache display not implemented yet \n"; 
            break;

        case 8: // test bfs 
            cout << "\n[SORRY] BFS not implemented yet \n";
            break;

        case 9: // exit
            cout << "\n                                   exiting    geocacher  . \n";
            cout << "                                initiating    cleanup    .\n\n\n";
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
