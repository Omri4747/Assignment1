#ifndef AGENT_H_
#define AGENT_H_
#include <vector>
#include "Tree.h"
#include <queue>
#include <iostream>
#include "Session.h"

class Session;

class Agent{
public:
    Agent();
    virtual ~Agent();
    virtual void act(Session& session)=0;

    virtual bool getFinished()=0;
    virtual Agent* clone() const=0;
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual void act(Session& session);
    virtual bool getFinished();
    virtual Agent* clone() const;
};


class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual void act(Session& session);
    virtual bool getFinished();
    virtual Agent* clone() const;

private:
    const int nodeInd;
    bool finished;
    void carryNextNeighbor(int node, Session& session);
    void infectCurrentNode(int node, Session& session);
};

#endif