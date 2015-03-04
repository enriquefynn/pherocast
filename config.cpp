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
    predictionFuture = root["predictionFuture"].asInt();
    ifs.close();
}
