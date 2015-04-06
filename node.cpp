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
    double angle = atan2(y - oy, x - ox);
    if (angle < 0)
        angle = 2*M_PI + angle;
    angle *= 180. / M_PI;
    if ((angle >= 315) || (angle < 45))
        return 'E';
    else if ((angle >= 45) && (angle < 135))
        return 'N';
    else if ((angle >= 135) && (angle < 225))
        return 'W';
    else if ((angle >= 225) && (angle < 315))
        return 'S';
    else
        return '?';
}

void Node::leave(long time)
{
    localWait+= time;
    if (avgWait == -1)
        avgWait = localWait;
    else
        avgWait = (1. - sigma)*avgWait + sigma*localWait; 
    localWait = 0; 
}

void Node::wait(long time)
{
    localWait += time;
}

void Node::enter(int tripID)
{
    int deltaTrip = tripID - lastTrip;
    timeCoef = timeCoef*pow(1. - sigma, deltaTrip) + sigma;
    lastTrip = tripID;
    localWait = 0;
}

/* Only look to timeCoef */
void Node::visit(int tripID)
{
    int deltaTrip = tripID - lastTrip;
    timeCoef = timeCoef*pow(1. - sigma, deltaTrip) + sigma;
    lastTrip = tripID;
}

