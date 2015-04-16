#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include <unistd.h>

#include "graph.hpp"
#include "node.hpp"
#include "config.hpp"

enum {NOT_SKIPPING, SKIPPING, SKIPPED};

bool distanceGap(Node* n1, Node* n2, double gapLimit){
    return (std::hypot(n1->getX() - n2->getX(), n1->getY() - n2->getY()) > gapLimit);
}

int main(int argc, char* argv[])
{
    unsigned int x, y, lastX, lastY, tripID, lastTrip;
    int yday, graphIdx;
    long ts, lastTs, timeToNextData, timeWaiting;
    double xl, yl;
    char opt;
    bool makingNewTrip, predictingWithTime;
    int skipThis; /*0: Not skipping 1: skipping 2: Skipped last*/

    std::vector<std::pair<double, Node*>> prediction;

    std::string configPath, fileName;

    configPath = "config.json";
    fileName = "default";
    std::ofstream statFileSize, statFileTime, graphFile;

    while ((opt = getopt(argc, argv, "c:n:")) != -1)
	{
		switch(opt)
		{
			case 'c':
                configPath = optarg;
                break;
            case 'n':
                fileName = optarg;
                break;
			default:
				puts("Receives the input from stdin\n\
					Options:\n\
						\t-c <config>: Config file\n\
						\t-n <file name>: File name\n\
						 ");
				exit(1);
		}
    }

    //Time test
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    Node *node = new Node(0, 0, 'N', 0), *oldNode = new Node(0, 0, 'N', 0), 
         *predictionNode = new Node(0, 0, 'N', 0);
    Config *config;
    try
    {
        config = new Config(configPath);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Config path not found, using default config.json" << 
            std::endl;
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
    timeToNextData = config->maxinterval;
    timeWaiting = tripID = graphIdx = 0;
    makingNewTrip = false;
    predictingWithTime = true;
    skipThis = NOT_SKIPPING;

    if (config->testTime)
        statFileTime.open(fileName + ".time");
    if (config->testSize)
        statFileSize.open(fileName + ".size");
    if (config->generateGraph)
        graphFile.open(fileName + ".graph");

    //Generic graph
    Graph *g = new Graph();

    //Time specific graphs
    std::vector<Graph*> timeSpecificGraphs;
    for (unsigned int ti = 0; ti < config->timeInterval.size(); ++ti)
        timeSpecificGraphs.push_back(new Graph());

    while(std::cin >> ts >> xl >> yl)
    {
        //It's in ms
        if (config->timeInMs)
            ts/= 1000;
        timeToNextData = ts - lastTs;

        time_t time = ts;
        auto formatedTime = std::gmtime(&time);
        //Get the correct node to compare/predict
        x = (int)std::nearbyint(config->multiplier*xl);
        y = (int)std::nearbyint(config->multiplier*yl);
        node->set(x, y, (config->useCoord) ? node->getDirection(x, y, lastX, lastY): 'N');

        //Get graphTime index, slow (but size is not big)
        for (unsigned int idx = 0; idx < config->timeInterval.size()-1; ++idx)
        {
            if (formatedTime->tm_hour >= config->timeInterval[idx] &&
                formatedTime->tm_hour < config->timeInterval[idx+1])
                {
                    graphIdx = idx;
                    break;
                }
        }

        //NEW trip at TODO: 04:00
        if (yday != formatedTime->tm_yday)
        {
            std::cerr << "Making new trip at: " << std::asctime(formatedTime);
            ++tripID;
            yday = formatedTime->tm_yday;
            timeToNextData = config->maxFuture;
            makingNewTrip = true;
        }
        else if (timeToNextData > config->maxinterval*config->interval)
        {
            std::cerr << "Not enough logs at: " << ts << ' ' << lastTs << 
                " deltaTS: " << timeToNextData << std::endl;
            skipThis = SKIPPING;
            timeToNextData = config->maxFuture;
        }
        else if (distanceGap(node, oldNode, config->gapLimit))
        {
            std::cerr << "Gap at " << ts << ' ' << lastTs << std::endl;
            skipThis = SKIPPING;
        }
        //timeToNextData = config->maxFuture;

        if (config->testTime)
            startTime = std::chrono::high_resolution_clock::now();

        if (skipThis == SKIPPED)
        {
            g->jumpToNode(node, tripID);
            timeSpecificGraphs[graphIdx]->jumpToNode(node, tripID);
        }

        if (skipThis == SKIPPED || skipThis == NOT_SKIPPING)
        {
            prediction = timeSpecificGraphs[graphIdx]->predictNexts(timeSpecificGraphs[graphIdx]->getNode(oldNode), timeWaiting, 
                    timeToNextData, tripID, phero::NOCIRCLE);
            if (config->testTime)
                statFileTime << std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now()-startTime).count() << std::endl;
        }
        
        double predictionProb = 0;
        if (makingNewTrip)
        {
            g->startNewTrip(node, tripID, timeToNextData);
            timeSpecificGraphs[graphIdx]->startNewTrip(node, tripID, timeToNextData);
        }

        if (skipThis == SKIPPED || skipThis == NOT_SKIPPING)
        {
            //Predict with time graphs
            for (unsigned int i = 0; i < prediction.size(); ++i)
            {
                if (i == config->maxopt)
                    break;
                if (prediction[i].second == timeSpecificGraphs[graphIdx]->getNode(node))
                {
                    predictionProb = prediction[i].first;
                    predictionNode = prediction[i].second;
                }
            }

            //Could not predict with time graph
            if (predictionProb == 0)
            {
                predictingWithTime = false;
                prediction = g->predictNexts(g->getNode(oldNode), timeWaiting, 
                    timeToNextData, tripID, phero::NOCIRCLE);
                predictionProb = 0;
                for (unsigned int i = 0; i < prediction.size(); ++i)
                {
                    if (i == config->maxopt)
                        break;
                    if (prediction[i].second == g->getNode(node))
                    {
                        predictionProb = prediction[i].first;
                        predictionNode = prediction[i].second;
                    }
                }

            }

            //Could not predict
            if (predictionProb == 0)
            {
                predictionProb = -1;
                predictionNode = oldNode;
            }
            //Waiting at the same node
            if (*node == *oldNode)
                timeWaiting+= timeToNextData;
            else
            {
                if (predictionProb == -1)
                    predictionProb = 0;
                timeWaiting = 0;
            }

            //Update the graph
            if (!makingNewTrip)
            {
                g->insert(node, tripID, timeToNextData);
                timeSpecificGraphs[graphIdx]->insert(node, tripID, timeToNextData);
            }
            //Testing size?
            if (config->testSize)
            {
                int totalSize = g->getSize();
                for (auto graph : timeSpecificGraphs)
                    totalSize+= graph->getSize();
                statFileSize << totalSize << std::endl;
            }
            
            if (skipThis == SKIPPED || makingNewTrip)
                std::cout << std::endl;
            if (!makingNewTrip)
                std::cout << predictionProb << ' ' << *oldNode << ' ' << *node <<
                    ' ' << ((predictionNode) ? predictionNode->getID() : "NULL") <<
                    ' ' << ts << ' ' << g->getNodesSize() << ' ' << tripID << 
                    std::endl;
        }

        //Update the past
        lastX = x;
        lastY = y;
        lastTrip = tripID;
        lastTs = ts;
        oldNode->set(node->getX(), node->getY(), node->getDirection());
        makingNewTrip = false;
        predictingWithTime = true;
        if (skipThis == SKIPPED)
            skipThis = NOT_SKIPPING;
        else if (skipThis == SKIPPING)
            skipThis = SKIPPED;
    }

    if (config->generateGraph)
    {
        g->printGraphviz(graphFile);
        for (unsigned int i = 0; i < timeSpecificGraphs.size(); ++i)
        {
            graphFile.close();
            std::cerr << "SSS: " << (fileName + std::to_string(i) + ".graph") << std::endl;
            graphFile.open(fileName + '.' + std::to_string(i) + ".graph");
            timeSpecificGraphs[i]->printGraphviz(graphFile);
        }
    }
    
    statFileTime.close();
    statFileSize.close();
    graphFile.close();

    //new line at EOF
    std::cout << std::endl;
}
