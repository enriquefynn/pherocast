#pragma once

#include <iostream>
#include "json/json.h"

struct Config{
    int interval = 15;
    int maxinterval = 30;
    int multiplier = 1000;
    int maxopt = 3;
    int predictionFuture = 2;
    bool timeInMs = true;

    Config(std::string configFile);
    friend std::ostream& operator<< (std::ostream &out, const Config &config)
    {
        out << "Using Config:" << 
        "\n\tInterval: " << config.interval << 
        "\n\tMaximum Inverval: " << config.maxinterval << 
        "\n\tMultiplier: " << config.multiplier << 
        "\n\tMaximum Opts: " << config.maxopt <<
        "\n\tPrediction in future: " << config.predictionFuture <<
        "\n\tTime in ms: " << config.timeInMs;
        return out;
    }
};
