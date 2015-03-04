#pragma once

#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "node.hpp"

namespace ants{
    const int SAMETRIP = 0;
    const int NEWTRIP = 1;
}

class Graph
{
    std::unordered_map<Node*, std::unordered_set<Node*>> graph;	/*The Graph*/
	Node *whereAmI = NULL;
    std::unordered_map<std::string, Node*> allNodes;

public:
	inline Graph(){}
	void insert(Node *node, int tripN, int flags);
	void print();
    
    inline Node* getNode(Node* node){return allNodes[node->getID()];}

    std::vector<Node*> dfs(std::unordered_map<Node*, bool> &visited, Node *node, double maxT, double localT);
    std::vector<std::pair<double, Node*>> predictNexts(Node* node, double timeSpentHere, double maxT, int tripID);

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
