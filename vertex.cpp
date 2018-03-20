/**
* By: Nathanael Fixx
* Each vertex has a unique label
* Can be connected to other vertices via weighted edges
* Cannot be connected to itself
* Used by depth-first search and breadth-first search
* to keep track of if it has been visited or not
*/

#include <climits>
#include "vertex.h"
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <algorithm>

#include "edge.h"


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** Creates an unvisited vertex, gives it a label, and clears its
adjacency list.
NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label)
{
	this->vertexLabel = label;

}

/** @return  The label of this vertex. */
std::string Vertex::getLabel() const
{
	return vertexLabel; 
}

/** Marks this vertex as visited. */
void Vertex::visit()
{
	visited = true;
}

/** Marks this vertex as not visited. */
void Vertex::unvisit()
{
	visited = false;
}

/** Returns the visited status of this vertex.
@return  True if the vertex has been visited, otherwise
returns false/ */
bool Vertex::isVisited() const
{ 
	if (visited == true)
		return true;
	else
		return false;
}

/** Adds an edge between this vertex and the given vertex.
Cannot have multiple connections to the same endVertex
Cannot connect back to itself
@return  True if the connection is successful. */
bool Vertex::connect(const std::string& endVertex, const int edgeWeight)
{ 
	Edge temp(endVertex, edgeWeight);

	adjacencyList.insert(std::pair<std::string, Edge>(endVertex, temp));
	currentNeighbor = adjacencyList.find(endVertex);

	return true;
	
}

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex)
{
	map<string, Edge>::iterator it;

	for (it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
		if (it->second.getEndVertex() == endVertex) {
			adjacencyList.erase(it);
			return true;
		}
	}
		
	return false;
}

/** Gets the weight of the edge between this vertex and the given vertex.
@return  The edge weight. This value is zero for an unweighted graph and
is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex)
{ 
	map<string, Edge>::iterator it;

	for (it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
		if (it->second.getEndVertex() == endVertex) {
			return it->second.getWeight();
		}
	}
	return -1;
}

/** Calculates how many neighbors this vertex has.
@return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors(){
	
	map<string, Edge>::iterator it;
	int neighborCount = 0;

	for (it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
		neighborCount++;
	}
	 
	return neighborCount;
}

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor()
{
	currentNeighbor = adjacencyList.begin();
}

/** Gets this vertex's next neighbor in the adjacency list.
Neighbors are automatically sorted alphabetically via map
Returns the vertex label if there are no more neighbors
@return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor() 
{ 
	map<string, Edge>::iterator it = adjacencyList.begin();
	int count = 0;

	if (adjacencyList.empty()) {
		return vertexLabel;
	}

	while (count != this->getNumberOfNeighbors() - 1) {
		it++;
		count++;
	}

	if (it == currentNeighbor) {
		return vertexLabel;
	}
	
	currentNeighbor++;

	return currentNeighbor->first;
}

/** Sees whether this vertex is equal to another one.
Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex& rightHandItem) const
{
	if (this->getLabel() == rightHandItem.getLabel()) {
		return true;
	}

	return false;
}

/** Sees whether this vertex is < another one.
Compares vertexLabel. */
bool Vertex::operator<(const Vertex& rightHandItem) const
{ 
	if (this->getLabel() < rightHandItem.getLabel()) {
		return true;
	}

	return false;
}

/**Used to get the first neighbor of a vertex, get next neighbor increments
currentNeighbor iterator so it can not be used to get the first neighbor*/
string Vertex::getCurrentNeighbor()
{
	map<string, Edge>::iterator it = adjacencyList.begin();
	int count = 0;

	// for empty list
	if (adjacencyList.empty()) {
		return vertexLabel;
	}

	// sets an iterator to the last value in the adjacency list to ensure
	// the currentNeighbor iterator does not go off the list 
	while (count != this->getNumberOfNeighbors()) {
		it++;
		count++;
	}

	if (it == currentNeighbor) {
		return vertexLabel;
	}


	return currentNeighbor->first;
}

/**returns the endVertex value for the edge*/
string Vertex::getEndVertex()
{
	if (this->getNumberOfNeighbors() == 0) {
		return this->getLabel();
		}
	
	currentNeighbor--;
	string endVertex = currentNeighbor->first;
	currentNeighbor++;
	return endVertex;
}