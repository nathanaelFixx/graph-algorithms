/**
* Nathanael Fixx
* A graph is made up of vertices and edges
* A vertex can be connected to other vertices via weighted, directed edge
* Functions allow for the construction and destrution of graph objects. The 
* user can also get the number of vertices, the number of edges, add a vertex
* to the graph, return the weight of an edge, read from a file, perform depth-
* first search, breadth-first search, and find Dijkstra's shortest path.
* Private functions allows user to find and create vertices and visit or
* unvisit vertices.
*/

#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include <string>

#include "vertex.h"
#include "edge.h"
#include <queue>

class Graph {
public:
	/** constructor, empty graph */
	Graph();

	/** destructor, delete all vertices and edges
	only vertices stored in map
	no pointers to edges created by graph */
	~Graph();

	/** return number of vertices */
	int getNumVertices() const;

	/** return number of edges */
	int getNumEdges() const;

	/** add a new edge between start and end vertex
	if the vertices do not exist, create them
	calls Vertex::connect
	a vertex cannot connect to itself
	or have multiple edges to another vertex */
	bool add(std::string start, std::string end, int edgeWeight = 0);

	/** return weight of the edge between start and end
	returns INT_MAX if not connected or vertices don't exist */
	int getEdgeWeight(std::string start, std::string end);

	/** read edges from file
	the first line of the file is an integer, indicating number of edges
	each edge line is in the form of "string string int"
	fromVertex  toVertex    edgeWeight */
	void readFile(std::string filename);

	/** depth-first traversal starting from startLabel
	call the function visit on each vertex label */
	void depthFirstTraversal(std::string startLabel,
		void visit(const std::string&));

	/** breadth-first traversal starting from startLabel
	call the function visit on each vertex label */
	void breadthFirstTraversal(std::string startLabel,
		void visit(const std::string&));

	/** find the lowest cost from startLabel to all vertices that 
	can be reached
	using Djikstra's shortest-path algorithm
	record costs in the given map weight
	weight["F"] = 10 indicates the cost to get to "F" is 10
	record the shortest path to each vertex using given map previous
	previous["F"] = "C" indicates get to "F" via "C"

	cpplint gives warning to use pointer instead of a non-const map
	which I am ignoring for readability */
	void djikstraCostToAllVertices(
		std::string startLabel,
		std::map<std::string, int>& weight,
		std::map<std::string, std::string>& previous);
	
private:

	/** used as an alternative comparison for priority queue */
	struct myComparison {
		bool operator()(pair<string, int> const & n1,
		 pair<string, int> const & n2) const noexcept {
			return n1.second > n2.second;
		}
	};

	/** number of vertices in graph */
	int numberOfVertices;

	/** number of edges in graph */
	int numberOfEdges;

	/** mapping from vertex label to vertex pointer for quick access */
	std::map<std::string, Vertex*> vertices;

	/** mark all verticies as unvisited */
	void unvisitVertices();

	/** find a vertex, if it does not exist return nullptr */
	Vertex* findVertex(const std::string& vertexLabel);

	/** find a vertex, if it does not exist create it and return it */
	Vertex* findOrCreateVertex(const std::string& vertexLabel);


	/** Helper functions for Dijkstra's path **/
	
	/** fills the weight map */
	void getWeight(Vertex * find, string startLabel,
	 priority_queue<pair<string, int>, vector<pair<string, int>>,
		 myComparison>&pq, std::map<std::string, int>& weight,
			 std::map<std::string, std::string>& previous);
	
	/** fills the previous map */
	void getPrevious(Vertex * find, string startLabel,
		 std::map<std::string, std::string>& previous);

	/** helps weight function calculate the total weight */
	int calculateWeight(std::map<std::string, int>& weight, string start);

	/** removes a weight from the weight map if a lower weight to the
	 same vertex is found */
	void removeWeight(std::map<std::string, int>& weight, string remove);
	
	/** removes the previous values from the map if the weight values
	 are removed */
	void removePrevious(std::map<std::string, string>& previous,
	 string remove);
	
	/** Checks if a value has already been added into the map */
	void findDuplicates(std::map<std::string, int>& weight, string find,
	 int challenger, std::map<std::string, string>& previous);


};  // end Graph

#endif  // GRAPH_H
