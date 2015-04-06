#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "graph.hpp"

Node* Graph::getNode(Node* node)
{
    if (!allNodes.count(node->getID()))
        return NULL;
    return allNodes[node->getID()];
}

void Graph::createNode(Node *node, int tripN, long deltaTime)
{
    if (allNodes.count(node->getID()))
        return;
    Node *newNode = new Node(node->getX(), node->getY(), node->getDirection(), tripN);
    allNodes[newNode->getID()] = newNode;
    //init graph's node
    //graph[newNode] = std::unordered_set<Node*>();
    //Update where I am
    if (whereAmI)
    {
        whereAmI->visit(tripN);
        graph[whereAmI].insert(newNode);
        whereAmI->leave(deltaTime);
    }
    whereAmI = newNode;

    //Update the size of the graph
    size+= sizeof(Node);
}

/** Inserts a node in the graph
 * \param x X coordinate
 * \param y Y coordinate
 * \param tripN The Nth trip
*/
void Graph::insert(Node *node, int tripN, long deltaTime)
{
	//There isn't a key yet
	if (!allNodes.count(node->getID()))
	{
        createNode(node, tripN, deltaTime);
        return;
	}
    //Are we waiting in the same node?
    if (*whereAmI == *node)
        whereAmI->wait(deltaTime);
    else
    {
        whereAmI->visit(tripN);
        whereAmI->leave(deltaTime);
        graph[whereAmI].insert(allNodes[node->getID()]);
        whereAmI = allNodes[node->getID()];
        whereAmI->enter(tripN);
        
        //Update the Size of the graph
        size+=sizeof(Node*);
    }
}

/** Starts a new trip at node ref node
 */
void Graph::startNewTrip(Node *node, int tripN, long deltaTime)
{
    if (!allNodes.count(node->getID()))
    {
        createNode(node, tripN, deltaTime);
        return;
    }
    whereAmI->leave(deltaTime);
    whereAmI = allNodes[node->getID()];
}

/** Predict where to be
 * \param node Node
 * \param maxT Maximum forecast time
 * \param localT Local time (for recursion)
*/
std::vector<Node*> Graph::dfs(std::unordered_map<Node*, bool> &visited, Node *node, double maxT, double localT, unsigned int flags)
{
    std::vector<Node*> probNodes;
    if (node == NULL)
        return probNodes;
    if ((flags & phero::NOCIRCLE) == phero::NOCIRCLE)
    {
        if (visited[node])
            return probNodes;
        visited[node] = true;
    }
    //std::cerr << "ENTERING: " << *node << " MaxT: " << maxT << " LocalT: " << localT << " AVG: " << node->getAvgWait() << std::endl;
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
        std::vector<Node*> dfsProbs = dfs(visited, child, maxT, localT + node->getAvgWait(), flags);
        probNodes.insert(probNodes.end(), dfsProbs.begin(), dfsProbs.end());
	}
    return probNodes;
}

std::vector<std::pair<double, Node*>> Graph::predictNexts(Node* node, double timeSpentHere, double maxT, int tripID, unsigned int flags)
{
    std::unordered_map<Node*, bool> visited;
    std::vector<std::pair<double, Node*>> probPairs;
    std::vector<Node*> probNodes = dfs(visited, node, timeSpentHere + maxT, 0, flags);
	double all = 0;
	for (auto no : probNodes)
	{
		no->visit(tripID);
		all+= no->timeCoef;
	}
	for (auto no : probNodes)
		probPairs.push_back(std::make_pair(no->timeCoef/all, no));
	sort(probPairs.begin(),probPairs.end());
	return probPairs;
}

void Graph::jumpToNode(Node* node, int tripN)
{
    if (!allNodes.count(node->getID()))
    {
        Node *newNode = new Node(node->getX(), node->getY(), node->getDirection(), tripN);
        allNodes[newNode->getID()] = newNode;
    }
    whereAmI = allNodes[node->getID()];
}

void Graph::printGraphviz(std::ofstream &out)
{
    std::string node_id;
    out << "digraph G{\n" <<
        "\trankdir=LR;\n";
    std::unordered_map<Node*, std::string> simplifiedIds;
    int id = 0;
    for (auto sa : allNodes)
    {
        if (sa.first != (*sa.second).getID())
            std::cerr << "Something wrong at node with ID: " << sa.first << std::endl;
        if (!simplifiedIds.count(sa.second))
            simplifiedIds[sa.second] = std::to_string(id++);
        node_id = simplifiedIds[sa.second];
        node_id = sa.second->getID();
        if (!graph[sa.second].empty())
        {
            auto node = graph[sa.second].begin();
            out << "\t" << node_id << "[label_scheme=" << node_id << '(' << sa.second->getAvgWait() << ')' << "];" << " -> ";
            unsigned int i = 0;
            for (; node != graph[sa.second].end(); node++)
            {
                if (i++ == graph[sa.second].size()-1)
                    break;
                if (!simplifiedIds.count(*node))
                    simplifiedIds[*node] = std::to_string(id++);
                node_id = simplifiedIds[*node];
                node_id = (*node)->getID();
                out << node_id << ", ";
            }
            if (!simplifiedIds.count(*node))
                simplifiedIds[*node] = std::to_string(id++);
            node_id = simplifiedIds[*node];
            node_id = (*node)->getID();
            out << node_id << ";\n";
        }
    }
    out << "}\n";
}

