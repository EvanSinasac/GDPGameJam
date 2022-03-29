#pragma once

#include "Vertex.h"
#include <vector>

using namespace std;

struct Node
{
	char id;
	bool visited;
	bool hasGoal;
	float costSoFar;
	float hDistance;
	Vertex position;					// position in our game world.  Used for calculating distances
	struct Node* parent;				// parent Node that we can use to follow back to the root node
	vector<pair<Node*, float>> edges;	// Edges pointing to our neighbouring nodes <neighbour, edgeWeight>
	char type;
	bool isHomeBase;
};

class Graph
{
public:
	vector<Node*> nodes;				// All the nodes belonging to this graph

	Graph();
	void CreateNode(char id, Vertex position, bool bHasGoal = false);
	void CreateNode(char id, Vertex position, char typ, bool isHomeBase, bool bHasGoal = false);
	void AddEdge(Node* origin, Node* destination, float weight, bool bUndirected = true);
	void PrintGraph();

	void ResetGraph();
	void PrintParents(bool includeCost = false);
};