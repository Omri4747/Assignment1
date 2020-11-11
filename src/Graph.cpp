#include "../include/Graph.h"

//constructor, everyone are healthy at the start (0 at infectin list is healthy)
Graph::Graph(vector<vector<int>> matrix):edges(matrix),infectionList(vector<int>((int)edges.size(), 0)){}

void Graph::infectNode(int nodeInd){
    infectionList[nodeInd]=2;
}

bool Graph::isInfected(int nodeInd) {
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
    for (int i = 0; i < (int) edges.size();i++) {
        if (edges[nodeInd][i]==1 && infectionList[i]==0)
            return i;
    }
    return -1;
}

vector<vector<int>> Graph::getEdges() {return edges;}

void Graph::setEdges(vector<vector<int>> _edges) {edges=_edges;}

vector<int> Graph::getInfectedNodes() {
    vector<int> output = vector<int>();
    for (int i = 0; i < (int)infectionList.size(); ++i) {
        if (infectionList[i]==2)
            output.push_back(i);
    }
    return output;
}