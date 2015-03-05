#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "graph.hpp"

void Graph::createNode(Node *node, int tripN)
{
    Node *newNode = new Node(node->getX(), node->getY(), node->getDirection(), tripN);
    allNodes[node->getID()] = newNode;
    //init graph's node
    graph[newNode] = std::unordered_set<Node*>();
    //Update where I am
    if (whereAmI)
    {
        graph[whereAmI].insert(newNode);
        whereAmI->leave();
    }
    whereAmI = newNode;
}

/** Inserts a node in the graph
 * \param x X coordinate
 * \param y Y coordinate
 * \param tripN The Nth trip
*/
void Graph::insert(Node *node, int tripN)
{
	//There isn't a key yet
	if (!allNodes.count(node->getID()))
	{
        createNode(node, tripN);
        return;
	}
    //Are we waiting in the same node?
    if (*whereAmI == *node)
        whereAmI->wait();
    else
    {
        whereAmI->enter(tripN);
        whereAmI->leave();
        graph[whereAmI].insert(allNodes[node->getID()]);
        whereAmI = allNodes[node->getID()];
        whereAmI->enter(tripN);
    }
}

/** Starts a new trip at node ref node
 */
void Graph::startNewTrip(Node *node, int tripN)
{
    if (!allNodes.count(node->getID()))
    {
        createNode(node, tripN);
        return;
    }
    whereAmI->leave();
    whereAmI = allNodes[node->getID()];
}

/** Predict where to be
 * \param node Node
 * \param maxT Maximum forecast time
 * \param localT Local time (for recursion)
*/
std::vector<Node*> Graph::dfs(std::unordered_map<Node*, bool> &visited, Node *node, double maxT, double localT, unsigned int flags)
{
    //std::cout << "ENTERING: " << *node << std::endl;
    std::vector<Node*> probNodes;
    if ((flags & phero::COUNTCIRCLE) == phero::COUNTCIRCLE)
    {
        if (visited[node])
            return probNodes;
        visited[node] = true;
    }
	if (node->getAvgWait() + localT > maxT)
    {
        probNodes.push_back(node);
		return probNodes;
    }
    if (graph[node].empty())
    {
        probNodes.push_back(node);
        return probNodes;
    }
	for (auto child : graph[node])
	{
        std::vector<Node*> dfsProbs = dfs(visited, child, maxT, localT + child->getAvgWait(), flags);
        probNodes.insert(probNodes.end(), dfsProbs.begin(), dfsProbs.end());
	}
    return probNodes;
}

std::vector<std::pair<double, Node*>> Graph::predictNexts(Node* node, double timeSpentHere, double maxT, int tripID, unsigned int flags)
{
    std::unordered_map<Node*, bool> visited;
    std::vector<std::pair<double, Node*>> probPairs;
    std::vector<Node*> probNodes = dfs(visited, node, maxT, 0, flags);
	double all = 0;
	for (auto no : probNodes)
	{
		no->visit(tripID);
		all+= no->timeCoef;
	}
	for (auto no : probNodes)
		probPairs.push_back(std::make_pair(node->timeCoef/all, no));
	sort(probPairs.begin(),probPairs.end());
    //std::cout << "PREDICTIC: ";
    //for (auto pair: probPairs)
    //    std::cout << pair.first << " " << *pair.second << std::endl;
	return probPairs;
}

void Graph::print()
{
	for (auto sa : graph)
	{
        std::cout << *sa.first << '(';
        std::cout << sa.first->getAvgWait() << "): ";
		for (auto node : sa.second)
			std::cout << *node << ' ';
        std::cout << std::endl;
	}
}

