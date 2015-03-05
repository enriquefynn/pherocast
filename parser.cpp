#include <iostream>
#include <chrono>

#include "graph.hpp"
#include "node.hpp"
#include "config.hpp"

int main(int argc, char* argv[])
{
    int x, y, lastX, lastY, tripID, lastTs, lastTrip, timeToNextData, yday;
    long ts;
    double xl, yl;
    Graph *g = new Graph();
    Node *node = new Node(0, 0, 'N', 0);
    Config *config;
    try
    {
        config = new Config(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Config path not found, using default config.json" << std::endl;
        try
        {
            config = new Config("config.json");
        }
        catch(const std::exception& e)
        {
            std::cerr << "Could not load config, exiting" << std::endl;
            exit(2);
        }
    }
    std::cerr << *config << std::endl;
    lastX = lastY = lastTs = lastTrip = yday = 0;
    timeToNextData = config->interval;
    tripID = 0;

    while(std::cin >> ts >> xl >> yl)
    {
        //It's in ms
        if (config->timeInMs)
            ts /= 1000;
        time_t time = ts;
        auto formatedTime = std::gmtime(&time);


        if (ts - lastTs > config->maxinterval*config->interval);

        x = config->multiplier*xl;
        y = config->multiplier*yl;
        node->set(x, y, node->getDirection(x, y, lastX, lastY));

        //NEW trip at TODO: 04:00
        if (yday != formatedTime->tm_yday)
        {
            std::cerr << "Making new trip at: " << std::asctime(formatedTime);
            ++tripID;
            g->startNewTrip(node, tripID);
            yday = formatedTime->tm_yday;
        }

        //Update the graph
        g->insert(node, tripID);
        //Try to predict
        g->predictNexts(g->getNode(node), node->getAvgWait(), timeToNextData, tripID, phero::NOCIRCLE);

        //Update the past
        lastTs = ts;
        lastX = x;
        lastY = y;
        lastTrip = tripID;
        timeToNextData = ts - lastTs;
    }
}
