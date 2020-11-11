#ifndef GRAPH_H_
#define GRAPH_H_
#include <vector>
using namespace std;

class Graph{
public:
    Graph(vector<vector<int>> matrix);  //CTR

    void infectNode(int nodeInd);   //Infect the node
    bool isInfected(int nodeInd);   //checks if the node is infected
    bool isCarrier(int nodeInd);    //checks if the node is a carrier
    void makeCarrier(int nodeInd);  //makes a node a carrier of the virus

    int nextHealthyNeighbor(int nodeInd);   //returns the next healthy neighbor with minimal index
    vector<vector<int>> getEdges();     //returns the edges of the graph
    void setEdges(vector<vector<int>> _edges);
    vector<int> getInfectedNodes();


private:
    vector<vector<int>> edges;
    vector<int> infectionList;  //0 is healthy, 1 is carrier, 2 is sick
};

#endif
