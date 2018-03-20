//_____________________________________________________________________________
// By: Nathanael Fixx
// 
// Edge Class. Creates an edge object to be used by a vertex object.
// Edges have a string endVertex and an int weight. Functions
// in this class allow for the creation of edge object. Also allows the user to
// get the value of the string, the value of the int, and output the edge
// values to the ostream
//_____________________________________________________________________________

#include <string>
#include <iostream>

#include "edge.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////

// constructor
Edge::Edge()
{
	
}

/** constructor with label and weight */
Edge::Edge(const std::string& end, int weight)
{
	this->endVertex = end;
	this->edgeWeight = weight;

}

/** return the vertex this edge connects to */
std::string Edge::getEndVertex() const
{ 
	return endVertex; 
}

/** return the weight/cost of travlleing via this edge */
int Edge::getWeight() const
{ 
	return edgeWeight;
}

ostream & operator<<(ostream & outstream, const Edge & e)
{
	outstream << e.getEndVertex() << e.getWeight();

	return outstream;
}
