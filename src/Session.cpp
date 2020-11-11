#include "../include/Session.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;
//normal constructor
/**
 *
 * @param path
 */

Session::Session(const string &path):g(vector<vector<int>>()),treeType(),
                    agents(vector<Agent*>()),currCycle(0),infectedQueue(queue<int>()){
    json data;
    ifstream inFileJson(path);
    inFileJson >> data;

    vector<vector<int>> matrix = data["graph"];
    g = Graph(matrix);
    string whichTree = data["tree"];
    if (whichTree=="M")
        treeType=MaxRank;
    else if (whichTree=="C")
        treeType=Cycle;
    else
        treeType=Root;
    for (int i = 0; i < (int) data["agents"].size(); ++i) {
        string agentType = data["agents"][i][0];
        if (agentType=="C"){
            ContactTracer toAdd = ContactTracer();
            addAgent(toAdd);
        }
        else{
            Virus toAdd = Virus(data["agents"][i][1]);
            addAgent(toAdd);
            g.makeCarrier(data["agents"][i][1]);
        }
    }
}
Session::Session(const Session& _session):g(_session.g),treeType(_session.treeType),agents(vector<Agent *>()),
                                            currCycle(_session.currCycle),infectedQueue(_session.infectedQueue) {
    for (Agent *agent : _session.agents) {
        addAgent(*agent);
    }
}

//move constructor
Session::Session(Session&& _session):g(_session.g),treeType(_session.treeType),agents(_session.agents),
                                     currCycle(_session.currCycle),infectedQueue(_session.infectedQueue) {
    for (int i = 0; i < (int) agents.size(); ++i) {
        _session.agents[i]=nullptr;
    }
}

Session::~Session() {
    clear();
}

//copy assignment operator
Session& Session::operator=(const Session& _session) {
    if (this != &_session) {
        g = _session.g;
        treeType = _session.treeType;
        currCycle = _session.currCycle;
        infectedQueue = _session.infectedQueue;
        clear();
        for (Agent *agent : agents) {
            addAgent(*agent);
        }
    }
    return *this;
}

//move assignment operator
const Session& Session::operator=(Session&& _session) {
    if (this != &_session) {
        g = _session.g;
        treeType = _session.treeType;
        currCycle = _session.currCycle;
        infectedQueue = _session.infectedQueue;
        clear();
        agents = _session.agents;
        for (int i = 0; i < (int) _session.agents.size(); ++i) {
            _session.agents[i] = nullptr;
        }
    }
    return *this;
}

void Session::clear() {
    for (Agent* p : agents)  {
        if(p) delete p;
    }
}

Graph Session::getGraph() const {return g;}
vector<Agent*> Session::getAgents() const {return agents;}
TreeType Session::getTreeType() const {return treeType;}

void Session::setGraph(const Graph &graph) {g=graph;}

void Session::addAgent(const Agent& agent) {
    agents.push_back(agent.clone());
}

int Session::nextHealthyNeighbor(int nodeInd) {return g.nextHealthyNeighbor(nodeInd);}

void Session::disconnectNode(int toRemove) {
    vector<vector<int>> edges = g.getEdges();
    for (int i = 0; i <(int)edges.size() ; ++i) {
        edges[toRemove][i] = 0;
        edges[i][toRemove] = 0;
    }
    g.setEdges(edges);
}

void Session::makeCarrier(int nodeInd) {
    g.makeCarrier(nodeInd);
    Virus toAdd = Virus(nodeInd);
    addAgent(toAdd);
}

void Session::simulate() {
    while (!allVirusFinished()){
        int currAgentsSize = (int) agents.size();
        for (int i = 0; i < currAgentsSize; ++i) {
            agents[i]->act(*this);
        }
        currCycle++;
    }
    json data;
    data["graph:"]=g.getEdges();
    data["infected:"]=g.getInfectedNodes();
    ofstream output("./output.json");
    output << data;
}

bool Session::allVirusFinished() {
    bool output= true;
    for (int i = 0; output && i < (int) agents.size(); ++i) {
            if (!agents[i]->getFinished())
                output = false;
        }
    return output;
}

int Session::dequeueInfected() {
    int output = infectedQueue.front(); //get first element
    infectedQueue.pop();    //remove first element
    return output;
}

void Session::enqueueInfected(int nodeInd) {
    g.infectNode(nodeInd);
    infectedQueue.push(nodeInd);
}

Tree* Session::BFS(int nodeInd) {
    Tree* root = Tree::createTree(*this,nodeInd);
    vector<vector<int>> edges = g.getEdges();
    vector<int> colors((int)edges.size(),0); //0 for white, 1 for gray, 2 for black from BFS algorithm
    colors[nodeInd]=1;
    queue<Tree*> Q = queue<Tree*>();
    Q.push(root);
    while (!Q.empty()){
        Tree* u = Q.front();
        Q.pop();
        for (int i = 0; i < (int) edges.size(); ++i) {
            if (edges[u->getRoot()][i]==1 && colors[i]==0) { //u and i are neighbors and not visited i yet
                colors[i] = 1;
                Tree* nextNeighbor = Tree::createTree(*this, i);
                u->addChild(nextNeighbor);
                Q.push(nextNeighbor);
            }
        }
        colors[u->getRoot()]=2;
    }
    return root;
}

int Session::getCurrCycle() const {return currCycle;}

bool Session::isQueueEmpty() {return infectedQueue.empty();}