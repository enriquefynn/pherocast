#pragma once

#include <iostream>

const double sigma = 0.4;

class Node
{
    int x, y;           /* Coordinades */
    char direction;     /*Heading direction */
    std::string id;

    long localWait = 0;  /* Local waiting time */
    double avgWait = -1.;/* Average waiting time */
    int lastTrip;       /* Last trip ID */

    void setID();

public:
    double timeCoef = 1;/* Times I got in this node */
    Node(int x, int y, char dir, int tripID);
    inline int getX() const {return x;}
    inline int getY() const {return y;}
    inline char getDirection() const {return direction;}
    inline void set(int x, int y, char dir) {this->x = x; this->y = y; this->direction = dir; setID();}

    inline std::string getID() const {return id;}
    inline double getAvgWait() {return avgWait;}
    void wait(long time);

    static char getDirection(int x, int y, int ox, int oy);

    void leave(long deltaTime);
    void enter(int tripId);
    void visit(int tripId);

    inline friend bool operator== (const Node &lhs, const Node &rhs){return ((lhs.x == rhs.x) && (lhs.y == rhs.y));}
	inline friend bool operator!= (const Node &lhs, const Node &rhs) {return !(lhs == rhs);}
	inline friend bool operator< (const Node &lhs, const Node &rhs){ return (lhs.x == rhs.x) ? (lhs.y < rhs.y): (lhs.x < rhs.x);}
	inline friend std::ostream& operator<< (std::ostream &out, const Node &node) {out << node.id; return out;}
};
