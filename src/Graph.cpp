#include "../include/Graph.h"
#include "../include/Tree.h"
#include "../include/Session.h"

//constructor, everyone are healthy at the start (0 at infectin list is healthy)
Graph::Graph(vector<vector<int>> matrix):edges(matrix),infectionList(vector<int>((int)edges.size(), 0)){}

void Graph::infectNode(int nodeInd){
    infectionList[nodeInd]=2;
}

 bool Graph::isInfected (int nodeInd){
    return infectionList[nodeInd]==2;
}

bool Graph::isCarrier(int nodeInd) {
    return infectionList[nodeInd]==1;
}

void Graph::makeCarrier(int nodeInd) {
    infectionList[nodeInd]=1;
}

//returns the next healthy neighbor of nodeInd with minimal index
int Graph::nextHealthyNeighbor(int nodeInd) {
    for (size_t i = 0; i < edges.size();i++) {
        if (edges[nodeInd][i]==1 && infectionList[i]==0)
            return i;
    }
    return -1;
}

const vector<vector<int>>& Graph::getEdges() {return edges;}

vector<int> Graph::getInfectedNodes() {
    vector<int> output = vector<int>();
    for (std::size_t i = 0; i < infectionList.size(); ++i) {
        if (infectionList[i]==2)
            output.push_back(i);
    }
    return output;
}

void Graph::removeEdge(int i, int j) {edges[i][j]=0;}

int Graph::graphSize() {return (int) edges.size();}

Tree * Graph::BFS(const Session& session, int nodeInd) {
    Tree* root = Tree::createTree(session,nodeInd);
    size_t graphSize = edges.size();
    vector<int> colors((int)graphSize,0); //0 for white, 1 for gray, 2 for black from BFS algorithm
    colors[nodeInd]=1;
    queue<Tree*> Q = queue<Tree*>();
    Q.push(root);
    while (!Q.empty()){
        Tree* u = Q.front();
        Q.pop();
        for (size_t i = 0; i < graphSize; ++i) {
            if (edges[u->getRoot()][i]==1 && colors[i]==0) { //u and i are neighbors and not visited i yet
                colors[i] = 1;
                Tree* nextNeighbor = Tree::createTree(session, i);
                u->addChild(nextNeighbor);
                Q.push(nextNeighbor);
            }
        }
        colors[u->getRoot()]=2;
    }
    return root;
}