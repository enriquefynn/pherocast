#include <sstream>
#include <cmath>
#include "node.hpp"

void Node::setID()
{
    std::stringstream ss;
    ss << x << ' ' << y << ' ' << direction;
    this->id = ss.str();
}

Node::Node(int x, int y, char dir, int tripID)
{
    this->x = x;
    this->y = y;
    this->direction = dir;
    this->lastTrip = tripID;
    setID();
}

char Node::getDirection(int x, int y, int ox, int oy)
{
    int angle = atan2(y - oy, ox - ox) * 180. / M_PI;
    if ((angle >= 315) || (angle < 45))
        return 'E';
    if ((angle >= 45) && (angle < 135))
        return 'N';
    if ((angle >= 135) && (angle < 225))
        return 'W';
    if ((angle >= 225) && (angle < 315))
        return 'S';
    return '?';
}

void Node::leave()
{
    avgWait = (1. - sigma)*avgWait + sigma*localWait; 
    localWait = 1; 
}

void Node::wait()
{
    ++localWait;
}

void Node::enter(int tripID)
{
    int deltaTrip = tripID - lastTrip;
    localWait = 1; 
    timeCoef = timeCoef*pow(1. - sigma, deltaTrip) + sigma;
    lastTrip = tripID;
}

/* Only look to timeCoef */
void Node::visit(int tripID)
{
    timeCoef = timeCoef*pow(1. - sigma, tripID - lastTrip);
    lastTrip = tripID;
}

