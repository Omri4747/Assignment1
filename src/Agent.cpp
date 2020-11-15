#include "../include/Agent.h"

Agent::Agent() {}

Agent::~Agent() {}

void ContactTracer::act(Session &session) {
    if (!session.isQueueEmpty()) {
        int infectedNode = session.dequeueInfected();
        Tree* infectedNodeTree = session.BFS(infectedNode);
        int toRemove = infectedNodeTree->traceTree();
        session.disconnectNode(toRemove);
        delete infectedNodeTree;
    }
}

ContactTracer::ContactTracer(){}

Agent* ContactTracer::clone() const {
    return new ContactTracer(*this);
}


Virus::Virus(int nodeInd):nodeInd(nodeInd), finished(false){}

void Virus::act(Session& session) {
    if (!finished) {
        if (session.isInfected(nodeInd)) {// node is sick
            carryNextNeighbor(nodeInd, session);
        } else {            //node is a carrier
            infectCurrentNode(nodeInd, session);
        }
    }
}

void Virus::infectCurrentNode(int node, Session& session) {
    session.enqueueInfected(node);         //push sick node to infectedQueue
    carryNextNeighbor(node,session);                   //infect the minimal next neighbor
}

void Virus::carryNextNeighbor(int node, Session& session) {
   int nextHealthyNeighbor = session.nextHealthyNeighbor(node); //get the next healthy neighbor to infect
   if (nextHealthyNeighbor!=-1){    //return -1 if no more healthy neighbors
       session.makeCarrier(nextHealthyNeighbor);      //make the next healthy node a carrier of the Virus
   }
   else {
       //no more healthy neighbors
       session.updateActiveViruses();
       finished = true;
   }
}

Agent* Virus::clone() const {
    return new Virus(*this);
}



