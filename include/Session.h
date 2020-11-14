#ifndef SESSION_H_
#define SESSION_H_
#include <vector>
#include <string>
#include <queue>
#include "Graph.h"
#include "Agent.h"
#include "Tree.h"
#include "json.hpp"
using namespace std;

class Agent;
class Tree;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const string& path); //CTR
    Session(const Session& _session); //copy CTR
    Session(Session&& _session); //move CTR
    virtual ~Session(); //destructor
    Session & operator=(const Session &_session); //assignment operator
    const Session& operator=(Session&& _session); //move assignment operator


    Graph getGraph() const;
    int getCurrCycle() const;
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    Tree* BFS(int nodeInd);
    void disconnectNode(int toRemove);
    int nextHealthyNeighbor(int nodeInd);
    void makeCarrier(int nodeInd);
    bool isQueueEmpty();
    void updateActiveViruses();
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;


    
private:
    //fields
    Graph g;
    TreeType treeType;
    vector<Agent*> agents;
    int currCycle;
    queue<int> infectedQueue;
    int activeVirusesCount;

    //methods
    void clear();
};

#endif
