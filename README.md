# graph-bfs

![language](https://img.shields.io/badge/language-C%2B%2B-blue) ![platform](https://img.shields.io/badge/platform-cross--platform-lightgrey)
<!-- ![CI](https://github.com/arikthehacker/graph-bfs/actions/workflows/ci.yml/badge.svg) -->

A directed graph stored as an adjacency list, with a breadth-first traversal. The sample
domain is a set of geocaches connected by distances; BFS produces a visit order from a
chosen start.

## quickstart

```
git clone https://github.com/arikthehacker/graph-bfs.git
cd graph-bfs
sudo apt-get install -y build-essential
make run
```

`make run` feeds `sample-input.txt` (build a 3-node graph, add edges, run BFS). It ends
with:

```
BFS geocache marathon order:
|  [0] A1 - start cache [VISITED]
|  [2] C3 - hill cache [VISITED]
|  [1] B2 - river cache [VISITED]

traversal path:
[0] -> [2] -> [1]
```

## menu

On start the program prints:

```
|||||||||||||||||||||||||||| geocache graph menu |||||||||||||||||||||||||||||||||
1. test constructor        2. test Destructor        3. build adjacency list
4. insert a vertex         5. insert an Edge         6. display adjacency list
7. display nearest cache   8. test breadth-first     9. exit
menu choice: 
```

## how it works

The graph is a `vector<adjList>`. Each vertex holds its data and a head pointer to a
linked list of `edgeNode`, so the whole structure is an array of linked lists:

```c
struct edgeNode{
    int index;          // index to connecting vertex
    double distance;    // how far to the next one in miles
    edgeNode* next;
};

struct adjList{
    string coords;
    string location_descrip;
    string hint;
    edgeNode* pointer;
};
```

The breadth-first traversal uses a `queue<int>` for the frontier and a `visited` vector.
It dequeues the current vertex, records it, pushes its unvisited neighbors, and recurses
on the next vertex in the queue:

```c
traversal_queue.pop();              // dequeue current geocache
marathon_order.push_back(current);  // record it
bfs_edges(vertices[current].pointer, traversal_queue, visited); // queue unvisited neighbors
if (!traversal_queue.empty())
    bfs(traversal_queue.front(), traversal_queue, visited, marathon_order);
```

The program is menu-driven and reads choices from standard input; `sample-input.txt` is
one scripted session and `expected-output.txt` is what it prints.
