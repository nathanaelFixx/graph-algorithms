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

#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <queue>
#include <vector>

#include "graph.h"

/**
* A graph is made up of vertices and edges
* A vertex can be connected to other vertices via weighted, directed edge
*/


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** constructor, empty graph */
Graph::Graph() 
{
	numberOfEdges = 0;
	numberOfVertices = 0;
}

/** destructor, delete all vertices and edges
only vertices stored in map
no pointers to edges created by graph */
Graph::~Graph()
{
	map<string, Vertex*>::iterator it;

	for (it = vertices.begin(); it != vertices.end(); ++it) {
			delete it->second;
	}

}

/** return number of vertices */
int Graph::getNumVertices() const
{
	
	return numberOfVertices; 
}

/** return number of vertices */
int Graph::getNumEdges() const
{ 
	return numberOfEdges;
}

/** add a new edge between start and end vertex
if the vertices do not exist, create them
calls Vertex::connect
a vertex cannot connect to itself
or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) {

	Vertex * added;
		if (findVertex(start)) {
			added = findVertex(start);
		}
	
		else {
			added = findOrCreateVertex(start);
			vertices.insert({ start, added });
		}

		Vertex * endVertex;
		if (!findVertex(end)) {
			endVertex = findOrCreateVertex(end);
			vertices.insert({ end, endVertex });
		}

		numberOfEdges++;
		return added->connect(end, edgeWeight);
}

/** return weight of the edge between start and end
returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end)
{ 
	map<string, Vertex*>::iterator it;

	for (it = vertices.begin(); it != vertices.end(); ++it) {
		if (it->first == start) {
			Vertex * weight = it->second;
			return weight->getEdgeWeight(end);
		}
	}

		return INT_MAX;
}

/** read edges from file
the first line of the file is an integer, indicating number of edges
each edge line is in the form of "string string int"
fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename)
{
	ifstream fin;
	int count = 0;
	fin.open(filename);
	int numEdges;
	string start;
	string end;
	int weight;

	fin >> numEdges;

	while (count < numEdges) {
		fin >> start;
		fin >> end;
		fin >> weight;

		add(start, end, weight);
		count++;
	}
	
}

/** depth-first traversal starting from startLabel
call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
 void visit(const std::string&))
{
	stack<string> dft;
	Vertex * temp;
	Vertex * check;
	int loopCount = 0;
	string neighbor;

	// empty map
	if (this->vertices.empty()) {
		return;
	}

	this->unvisitVertices();
	dft.push(startLabel);

	findVertex(startLabel)->visit();
	visit(startLabel);

	// if starter vertex has no neighbors
	if (findVertex(startLabel)->getNumberOfNeighbors() == 0) {
		return;
	}

	while (!dft.empty()) {

		temp = findVertex(dft.top());
		temp->resetNeighbor();

		// loops until it finds an unvisited neighbor
		while (loopCount < temp->getNumberOfNeighbors()) {
			neighbor = temp->getCurrentNeighbor();
			check = findVertex(neighbor);
			
			if (check->isVisited()) {
				neighbor = temp->getNextNeighbor();
				check = findVertex(neighbor);
				loopCount++;
			}
			else {
				break;
			}
		}

		temp = findVertex(neighbor);
		loopCount = 0;

		// pushes onto stack
		if (temp->isVisited() == false) {
			dft.push(neighbor);
			temp->visit();
			visit(neighbor);
		}
		else {
			dft.pop();
		}
	}

}

/** breadth-first traversal starting from startLabel
call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
 void visit(const std::string&))
{
	int loopCount = 0;
	string neighbor;
	Vertex * check;
	queue<string>bft;
	Vertex * temp;
	this->unvisitVertices();

	// empty map
	if (this->vertices.empty()) {
		return;
	}

	bft.push(startLabel);

	findVertex(startLabel)->visit();

	// if starter vertex has no neighbors
	if (findVertex(startLabel)->getNumberOfNeighbors() == 0) {
		visit(bft.front());
		return;
	}

	while (!bft.empty()) {

		temp = findVertex(bft.front());
		temp->resetNeighbor();

		// loops until it finds an unvisited neighbor
		while (loopCount < temp->getNumberOfNeighbors()) {
			neighbor = temp->getCurrentNeighbor();
			check = findVertex(neighbor);

			if (check->isVisited()) {
				neighbor = temp->getNextNeighbor();
				check = findVertex(neighbor);
				loopCount++;
			}
			else {
				bft.push(neighbor);
				findVertex(neighbor)->visit();
			}
		}

		//calls visit on the top value in the queue and then pops it
		temp = findVertex(neighbor);
		loopCount = 0;
		visit(bft.front());
		bft.pop();

	}
}

/** find the lowest cost from startLabel to all vertices that can be reached
using Djikstra's shortest-path algorithm
record costs in the given map weight
weight["F"] = 10 indicates the cost to get to "F" is 10
record the shortest path to each vertex using given map previous
previous["F"] = "C" indicates get to "F" via "C"

cpplint gives warning to use pointer instead of a non-const map 
which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(std::string startLabel,
 std::map<std::string, int>& weight, std::map<std::string,
 std::string>& previous)
{	
	weight.clear();
	previous.clear();
	this->unvisitVertices();
	
	// holds possible neighbors that have not been used yet
	priority_queue<pair<string, int>, vector<pair<string, int>>,
	 myComparison>pq;
	
	// list of vertices visited
	vector<string>vertexSet;
	
	int i = 0;
	string next;
	Vertex * find = findVertex(startLabel);
	find->visit();
	int size = 0;

	vertexSet.push_back(startLabel);
	size++;

	// loops until every vertex has been visited
	while (size < this->getNumVertices()) {
		
		Vertex * temp = findVertex(vertexSet[vertexSet.size() - 1]);
		temp->resetNeighbor();
		string start = temp->getLabel();

		// loops until all neighbors have been visited
		while (i < temp->getNumberOfNeighbors()) {
			
			// special case to get first neighbor
			if (i == 0) {
				next = temp->getCurrentNeighbor();
				find = findVertex(next);
			}

			else {
				next = temp->getNextNeighbor();
				find = findVertex(next);
			}

			// special case: protects against the starter vertex
			// being added to any of the maps
			if (next != startLabel) {
				getWeight(find, start, pq, weight, previous);
				getPrevious(find, start, previous);
			}
			i++;
		}

		// if the queue is empty, all possible vertices have been
		// reached
		if (pq.empty()) {
			return;
		}
		pair<string, int> smallest = pq.top();
		pq.pop();
		vertexSet.push_back(get<0>(smallest));
		size++;
		i = 0;
	}
}

/** Helper function for Dijkstra. Fills the weight map.
 Checks if the map already has that value in it */
void Graph::getWeight(Vertex * find, string startLabel,
 priority_queue<pair<string, int>, vector<pair<string, int>>, myComparison>&pq,
 std::map<std::string, int>& weight, std::map<std::string,
 std::string>& previous)
{
	
	string destination;
	string start = startLabel;

	destination = find->getLabel();
	int cost = this->getEdgeWeight(start, destination);
	cost += calculateWeight(weight, start);
	findVertex(destination)->visit();

	findDuplicates(weight, destination, cost, previous);
	pq.push(make_pair(destination, cost));

	weight.insert({ destination, cost });
}


/** helper fuction for Dijkstra, fills the previous map */
void Graph::getPrevious(Vertex * find, string startLabel, std::map<std::string,
 std::string>& previous)
{
	string start = startLabel;
	string destination;

	destination = find->getLabel();
	previous.insert({ destination, start });
	findVertex(destination)->visit();
}

/** helper function for getWeight, if a vertex is multiple vertices away from
     the starter node it adds up the total weights */
int Graph::calculateWeight(std::map<std::string, int>& weight, string start)
{
	int newWeight = 0;
	string check;
	map<string, int>::iterator it;

		for (it = weight.begin(); it != weight.end(); ++it) {
			if (it->first == start) {
				newWeight += it->second;
				check = it->first;
			}
		}


	return newWeight;
}

/* Helper function for getWeight. Removes a specific value from the map */
void Graph::removeWeight(std::map<std::string, int>& weight, string remove)
{
	map<string, int>::iterator it;

	for (it = weight.begin(); it != weight.end(); ++it) {
		if (it->first == remove) {
			weight.erase(it);
			return;
		}
	}
}

/* Helper function for getPrevious. Removes a specific value from the map */
void Graph::removePrevious(std::map<std::string, string>& previous,
 string remove)
{
	map<string, string>::iterator it;

	for (it = previous.begin(); it != previous.end(); ++it) {
		if (it->first == remove) {
			previous.erase(it);
			return;
		}
	}
}

/* Checks if a value is already in the weight map. If it is, it removes it
   and also removes that value from the previous map */
void Graph::findDuplicates(std::map<std::string, int>& weight, string find,
 int challenger, std::map<std::string, string>& previous)
{
	map<string, int>::iterator it;

	for (it = weight.begin(); it != weight.end(); ++it) {
		if (it->first == find) {
			if (it->second > challenger) {
				string remove = it->first;
				removeWeight(weight, remove);
				removePrevious(previous, remove);
				return;
			}
		}
	}
}


/** mark all verticies as unvisited */
void Graph::unvisitVertices()
{
	map<string, Vertex*>::iterator it;

	for (it = vertices.begin(); it != vertices.end(); ++it) {
		it->second->unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) {
	 
	map<string, Vertex*>::iterator it;

	it = vertices.find(vertexLabel);
	if (it != vertices.end()) {
		return it->second;
	}

	else {
		return NULL;
	}
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) {
	
	map<string, Vertex*>::iterator it;

	it = vertices.find(vertexLabel);
	if (it != vertices.end()) {
		return it->second;
	}

	else {
		Vertex * temp = new Vertex(vertexLabel);
		numberOfVertices++;
		return temp;
	}
}



