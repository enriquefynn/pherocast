#include <fstream>
#include "config.hpp"

Config::Config(std::string filestr)
{
    Json::Value root;
    std::ifstream ifs;
    ifs.open(filestr, std::ifstream::in);
    ifs >> root;
    interval = root["interval"].asInt();
    maxinterval = root["maxinterval"].asInt();
    multiplier = root["multiplier"].asDouble();
    gapLimit = root["gapLimit"].asDouble();
    maxopt = root["maxopt"].asInt();
    maxFuture = root["maxFuture"].asInt();
    timeInMs = root["timeInMs"].asBool();
    testTime = root["testTime"].asBool();
    testSize = root["testSize"].asBool();
    useCoord = root["useCoord"].asBool();
    generateGraph = root["generateGraph"].asBool();
    Json::Value intervals = root["timeInterval"];
    for (auto i : intervals)
        timeInterval.push_back(i.asInt());
    ifs.close();
}
