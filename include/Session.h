#ifndef SESSION_H_
#define SESSION_H_
#include <vector>
#include <string>
#include <queue>
#include "Graph.h"

#include "json.hpp"
using namespace std;

class Agent;
class Tree;
class Graph;

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
    Session& operator=(Session&& _session); //move assignment operator


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
    const TreeType& getTreeType() const;
    bool isInfected (int nodeInd);


    
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
