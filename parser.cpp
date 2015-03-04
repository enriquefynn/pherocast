#include <iostream>

#include "graph.hpp"
#include "node.hpp"

int main(int argc, char* argv[])
{
    int x, y;
    long ts;
    double xl, yl;
    Graph *g = new Graph();
    Node *node = new Node(0, 0, 'N', 0);

    char opt;

    while(std::cin >> ts >> xl >> yl)
    {
        x = 1000*xl;
        y = 1000*yl;
        node->set(x, y, node->getDirection(x, y, xl, yl));
        std::cout << "NODEZ: " << *node << std::endl;
        g->insert(node, 0, ants::SAMETRIP);
        g->print();
        g->predictNexts(g->getNode(node), node->getAvgWait(), 10, 0);
    }
}
