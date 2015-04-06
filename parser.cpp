#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include <unistd.h>

#include "graph.hpp"
#include "node.hpp"
#include "config.hpp"

enum {NOT_SKIPPING, SKIPPING, SKIPPED};

int main(int argc, char* argv[])
{
    unsigned int x, y, lastX, lastY, tripID, lastTrip;
    int yday;
    long ts, lastTs, timeToNextData, timeWaiting;
    double xl, yl;
    char opt;
    bool makingNewTrip;
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

    Graph *g = new Graph();
    Node *node = new Node(0, 0, 'N', 0), *oldNode = new Node(0, 0, 'N', 0), *predictionNode = new Node(0, 0, 'N', 0);
    Config *config;
    try
    {
        config = new Config(configPath);
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
    timeToNextData = config->maxinterval;
    timeWaiting = 0;
    tripID = 0;
    makingNewTrip = false;
    skipThis = NOT_SKIPPING;

    if (config->testTime)
        statFileTime.open(fileName + ".time");
    if (config->testSize)
        statFileSize.open(fileName + ".size");
    if (config->generateGraph)
        graphFile.open(fileName + ".graph");

    while(std::cin >> ts >> xl >> yl)
    {
        //It's in ms
        if (config->timeInMs)
            ts/= 1000;
        timeToNextData = ts - lastTs;

        time_t time = ts;
        auto formatedTime = std::gmtime(&time);

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
            std::cerr << "Not enough logs at: " << ts << ' ' << lastTs << " deltaTS: " << timeToNextData << std::endl;
            skipThis = SKIPPING;
            timeToNextData = config->maxFuture;
        }
        //timeToNextData = config->maxFuture;

        if (config->testTime)
            startTime = std::chrono::high_resolution_clock::now();

        if (skipThis == SKIPPED)
            g->jumpToNode(node, tripID);

        if (skipThis == SKIPPED || skipThis == NOT_SKIPPING)
        {
            prediction = g->predictNexts(g->getNode(node), timeWaiting, timeToNextData, tripID, phero::NOCIRCLE);
            if (config->testTime)
                statFileTime << std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now()-startTime).count() << std::endl;
        }
        
        //Get the correct node to compare/predict
        x = (int)std::nearbyint(config->multiplier*xl);
        y = (int)std::nearbyint(config->multiplier*yl);
        node->set(x, y, (config->useCoord) ? node->getDirection(x, y, lastX, lastY): 'N');

        double predictionProb = 0;
        if (makingNewTrip)
            g->startNewTrip(node, tripID, timeToNextData);

        if (skipThis == SKIPPED || skipThis == NOT_SKIPPING)
        {
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
                g->insert(node, tripID, timeToNextData);
            //Testing size?
            if (config->testSize)
                statFileSize << g->getSize() << std::endl;

            if (skipThis == SKIPPED)
                std::cout << std::endl;
            std::cout << predictionProb << ' ' << *oldNode << ' ' << *node << ' ' << 
                ((predictionNode) ? predictionNode->getID() : "NULL") << ' ' << ts << ' ' << 
                g->getNodesSize() << ' ' << tripID << std::endl;
        }

        //Update the past
        lastX = x;
        lastY = y;
        lastTrip = tripID;
        lastTs = ts;
        oldNode->set(node->getX(), node->getY(), node->getDirection());
        makingNewTrip = false;
        if (skipThis == SKIPPED)
            skipThis = NOT_SKIPPING;
        else if (skipThis == SKIPPING)
            skipThis = SKIPPED;
    }

    if (config->generateGraph)
        g->printGraphviz(graphFile);
    
    statFileTime.close();
    statFileSize.close();
    graphFile.close();

    //new line at EOF
    std::cout << std::endl;
}
