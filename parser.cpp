#include <iostream>
#include <chrono>
#include <cmath>

#include "graph.hpp"
#include "node.hpp"
#include "config.hpp"

int main(int argc, char* argv[])
{
    unsigned int x, y, lastX, lastY, tripID, lastTrip;
    int yday;
    long ts, lastTs, timeToNextData;
    double xl, yl;
    bool makingNewTrip = false;

    Graph *g = new Graph();
    Node *node = new Node(0, 0, 'N', 0), *oldNode = new Node(0, 0, 'N', 0), *predictionNode = new Node(0, 0, 'N', 0);
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
        catch(const char *ex){
            std::cerr << ex << std::endl;
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
            ts/= 1000;

        time_t time = ts;
        auto formatedTime = std::gmtime(&time);

        //NEW trip at TODO: 04:00
        if (yday != formatedTime->tm_yday)
        {
            std::cerr << "Making new trip at: " << std::asctime(formatedTime);
            ++tripID;
            makingNewTrip = true;
            g->startNewTrip(node, tripID);
            yday = formatedTime->tm_yday;
        }
        if (!makingNewTrip && timeToNextData > config->maxinterval*config->interval)
        {
            std::cerr << "Something strange at ts: " << ts << " deltaTS: " << timeToNextData << std::endl;
            makingNewTrip = false;
        }

        //Try to predict
        if (timeToNextData > config->maxFuture)
            timeToNextData = config->maxFuture;
        auto prediction = g->predictNexts(g->getNode(node), node->getAvgWait(), timeToNextData, tripID, phero::NOCIRCLE);
        
        predictionNode->set(node->getX(), node->getY(), node->getDirection());
        //Get the correct node to compare/predict
        x = config->multiplier*xl;
        y = config->multiplier*yl;
        node->set(x, y, node->getDirection(x, y, lastX, lastY));

        double predictionProb = 0;
        for (unsigned int i = 0; i < prediction.size(); ++i)
        {
            if (i > config->maxopt)
                break;
            if (prediction[i].second == g->getNode(node))
            {
                predictionProb = prediction[i].first;
                predictionNode = prediction[i].second;
            }
        }
        if (predictionProb == 0 && *node == *predictionNode)
            predictionProb = -1;

        //Update the graph
        g->insert(node, tripID);

        std::cout << predictionProb << ' ' << *oldNode << ' ' << *node << ' ' << 
            ((predictionNode) ? predictionNode->getID() : "NULL") << ' ' << ts << ' ' << tripID << std::endl;

        //Update the past
        lastX = x;
        lastY = y;
        lastTrip = tripID;
        timeToNextData = ts - lastTs;
        lastTs = ts;
        oldNode->set(node->getX(), node->getY(), node->getDirection());
    }
}
