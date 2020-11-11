#ifndef TREE_H_
#define TREE_H_
#include "Session.h"
#include "Graph.h"
#include <vector>
#include <map>
using namespace std;
class Session;

class Tree{
public:
    Tree(int rootLabel);
    Tree(const Tree& _Tree); //copy constructor
    virtual ~Tree(); //destructor
    Tree(Tree&& _Tree); //move constructor
    Tree& operator=(const Tree& _Tree); //assignment operator
    const Tree& operator=(Tree&& Tree); //move assignment operator

    void addChild(const Tree& child);   //add a child to the tree
    void addChild(Tree* child);
    vector<Tree*> getChildren();        //returns the children field

    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;
    int getRoot() const;

private:
    int node;
    vector<Tree*> children;
    void clear();
    virtual Tree* clone() const=0;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);

    virtual int traceTree();

private:
    int currCycle;
    virtual Tree* clone() const;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();

private:
    virtual Tree* clone() const;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();

private:
    virtual Tree* clone() const;
};

#endif
