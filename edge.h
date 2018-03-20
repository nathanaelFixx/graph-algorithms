//_____________________________________________________________________________
// By: Nathanael Fixx
// 
// Edge Class. Creates an edge object to be used by a vertex object.
// Edges have a string endVertex and an int weight. Functions
// in this class allow for the creation of edge object. Also allows the user to
// get the value of the string, the value of the int, and output the edge
// values to the ostream
//_____________________________________________________________________________

#ifndef EDGE_H
#define EDGE_H

#include <string>
#include <iostream>
using namespace std;

class Edge {
public:
	/** empty edge constructor */
	Edge();

	/** constructor with label and weight */
	Edge(const std::string& end, int weight);

	/** return the vertex this edge connects to */
	std::string getEndVertex() const;

	/** return the weight/cost of travlleing via this edge */
	int getWeight() const;

	friend ostream& operator<<(ostream & outstream, const Edge & e);

private:
	/** end vertex, cannot be changed */
	std::string endVertex{ "" };

	/** edge weight, cannot be changed */
	int edgeWeight{ 0 };
};  //  end Edge


#endif  // EDGE_H
