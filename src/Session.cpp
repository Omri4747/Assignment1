#include "../include/Session.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;


/**
 *Session CTR
 * @param path: const reference for path to input config file(json)
 * @init_member_list: initialize Session fields
 * 1. Graph CTR
 * 2. treeType
 * 3. agents list(vector) stores the Viruses & Contact tracers by pointers to the heap
 * 4. currCycle(int) for the number of the current Cycle in the simulate fuunction
 * 5. infectedQueue(Queue- include package in Session.h) stores the infected nodes each cycle
 */

Session::Session(const string &path):g(vector<vector<int>>()),treeType(),
                    agents(vector<Agent*>()),currCycle(0),infectedQueue(queue<int>()){

    //from path- convert the json file to accessible object- see documentation of nlohman at json.hpp
    json data;
    ifstream inFileJson(path);
    inFileJson >> data;

    // initializing parameters bythe values of the json config file
    vector<vector<int>> matrix = data["graph"];
    g = Graph(matrix);
    string whichTree = data["tree"];
    if (whichTree=="M")
        treeType=MaxRank;
    else if (whichTree=="C")
        treeType=Cycle;
    else
        treeType=Root;
    //initializing two types of agents- Virus & Contact tracer
    for (int i = 0; i < (int) data["agents"].size(); ++i) {
        string agentType = data["agents"][i][0];
        if (agentType=="C"){
            ContactTracer toAdd = ContactTracer(); //Contact tracer CTR
            addAgent(toAdd); // uses Contact tracer clone() method to init values on the heap
        }
        else{
            Virus toAdd = Virus(data["agents"][i][1]); //Virus CTR
            addAgent(toAdd); // uses Virus clone() method to init values on the heap
            g.makeCarrier(data["agents"][i][1]); // makes the first node carried
        }
    }
}

/**
 * Session copy CTR
 * @param _session: const reference of the Session obj
 * @init_member_list: initialize Session fields:
 * same as the CTR but takes the fields of the reference variable
 * for agent field- make deep copy by accessing the values on the heap
 */

Session::Session(const Session& _session):g(_session.g),treeType(_session.treeType),agents(vector<Agent *>()),
                                            currCycle(_session.currCycle),infectedQueue(_session.infectedQueue) {
    for (Agent *agent : _session.agents) {
        addAgent(*agent);
    }
}

/**
 * move CTR
 * @param _session: reference to the moved parameter
 * same as copy CTR but makes the values of the moved parameter nullptr
 * notice that the reference will be deleted at the end of the method
 */

Session::Session(Session&& _session):g(_session.g),treeType(_session.treeType),agents(_session.agents),
                                     currCycle(_session.currCycle),infectedQueue(_session.infectedQueue) {
    for (int i = 0; i < (int) agents.size(); ++i) {
        _session.agents[i]=nullptr;
    }
}

/**
 * Destructor
 * uses the clear method to avoid code duplicate
 */

Session::~Session() {
    clear();
}

/**
 * Assignment operator
 * @param _session: const reference to the Session object to assign
 * @return: return reference to the re-assigned Session object
 */

Session& Session::operator=(const Session& _session) {
    //check for self assignment
    if (this != &_session) {

        //assign the values to the Session object- except agents that mange resources on the heap
        g = _session.g;
        treeType = _session.treeType;
        currCycle = _session.currCycle;
        infectedQueue = _session.infectedQueue;

        //delete old values (agents) from the heap
        clear();

        //assign new values to agent field
        for (Agent *agent : agents) {
            addAgent(*agent);
        }
    }
    return *this;
}

/**
 * move assignment operator
 * @param _session: reference to the moves parameter
 * @return const reference to the re-assigned Session
 */

const Session& Session::operator=(Session&& _session) {
    //check for self assignment
    if (this != &_session) {

        //assign the values to the Session object- except agents, that manage resources on the heap
        g = _session.g;
        treeType = _session.treeType;
        currCycle = _session.currCycle;
        infectedQueue = _session.infectedQueue;

        //delete old values (agents) from the heap
        clear();

        //init agents value
        agents = _session.agents;

        //make the reference (Session) agents field point to nullptr
        for (int i = 0; i < (int) _session.agents.size(); ++i) {
            _session.agents[i] = nullptr;
        }
    }
    return *this;
}

/**
 * private method that called from destructor, assignment & move assignment operator
 * in charge of deleting the agents that stored on the heap
 */

void Session::clear() {
    for (Agent* p : agents)  {
        // check if p(agent) is not nullptr before delete
        if(p) delete p;
    }
}

/**
 * public getter method
 * @return: Graph field
 */

Graph Session::getGraph() const {return g;}

/**
 * public const getter method
 * @return: agents field
 */

vector<Agent*> Session::getAgents() const {return agents;}

/**
 * public const getter method
 * @return: treeType field (Cycle, MaxRank, Root)
 */

TreeType Session::getTreeType() const {return treeType;}

/*=========UNUSED======== */
void Session::setGraph(const Graph &graph) {g=graph;}

/**
 * @param agent: agent to add
 * uses the clone() method to create the agent on the heap and store the pointer in the agents field
 */

void Session::addAgent(const Agent& agent) {
    agents.push_back(agent.clone());
}

/**
 * callin the Graph method to do the job
 * @param nodeInd : the nodes that we look in its neighbors
 * @return: the minimal un-infected (healthy) node
 */

int Session::nextHealthyNeighbor(int nodeInd) {return g.nextHealthyNeighbor(nodeInd);}

/**
 * after Contact tracer trace tree, need to cut the chain of infection,
 * by disconnect the  node from all of its neighbors
 * @param toRemove: node to disconnect from all its neighbors
 */

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
    data["graph"]=g.getEdges();
    data["infected"]=g.getInfectedNodes();
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