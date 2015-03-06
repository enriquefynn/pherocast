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
    multiplier = root["multiplier"].asInt();
    maxopt = root["maxopt"].asInt();
    maxFuture = root["maxFuture"].asInt();
    timeInMs = root["timeInMs"].asBool();
    testTime = root["testTime"].asBool();
    testSize = root["testSize"].asBool();
    if (testTime && testSize)
        throw "ERROR: Can\'t test both size and time (testTime and testSize)";
    ifs.close();
}
