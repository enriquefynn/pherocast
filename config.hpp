#pragma once

#include <iostream>
#include "json/json.h"

struct Config{
    unsigned int interval = 15;
    unsigned int maxinterval = 30;
    double multiplier = 1000;
    unsigned int maxopt = 3;
    unsigned int maxFuture = 2;
    bool timeInMs = true;
    bool testTime = false;
    bool testSize = false;
    bool useCoord = false;

    Config(std::string configFile);
    friend std::ostream& operator<< (std::ostream &out, const Config &config)
    {
        out << "Using Config:" << 
        "\n\tInterval: " << config.interval << 
        "\n\tMaximum Inverval: " << config.maxinterval << 
        "\n\tMultiplier: " << config.multiplier << 
        "\n\tMaximum Opts: " << config.maxopt <<
        "\n\tPrediction in future: " << config.maxFuture <<
        "\n\tTime in ms: " << ((config.timeInMs) ? "True" : "False") <<
        "\n\tUsing NSWE coordinades: " << ((config.useCoord) ? "True" : "False");
        if (config.testTime)
            out << "\n\tTesting time";
        if (config.testSize)
            out << "\n\tTesting size";
        return out;
    }
};
