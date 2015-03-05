#pragma once

#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "node.hpp"

namespace phero{
    const unsigned int SAMETRIP = 1u << 0;
    const unsigned int NEWTRIP = 1u << 1;
    const unsigned int COUNTCIRCLE = 1u << 2;
    const unsigned int NOCIRCLE = 1u << 3;
}

class Graph
{
    std::unordered_map<Node*, std::unordered_set<Node*>> graph;	/*The Graph*/
	Node *whereAmI = NULL;
    std::unordered_map<std::string, Node*> allNodes;

    std::vector<Node*> dfs(std::unordered_map<Node*, bool> &visited, Node *node, double maxT, double localT, unsigned int flags);
    void createNode(Node *node, int tripN);

public:
	inline Graph(){}
	void insert(Node *node, int tripN);
    void startNewTrip(Node *node, int tripN);

	void print();
    
    inline Node* getNode(Node* node){return allNodes[node->getID()];}

    std::vector<std::pair<double, Node*>> predictNexts(Node* node, double timeSpentHere, double maxT, int tripID, unsigned int flags);

    inline int getSize(){
        int size = 0;
        for (auto it: graph)
        {
            size+= sizeof(graph[it.first]);
            for (auto node : it.second)
                size+= sizeof(node);
        }
        return size;
    }
};
