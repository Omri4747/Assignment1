#include "../include/Tree.h"
#include "../include/Session.h"



//constructor
Tree::Tree(int rootLabel):node(rootLabel),children(vector<Tree*>(0,0)){}

//copy constructor
Tree::Tree(const Tree& _Tree):node(_Tree.node),children(vector<Tree*>()) {
    for(Tree* child : _Tree.children){
        addChild(*child);
    }
}

//move constructor
Tree::Tree(Tree &&_Tree):node(_Tree.node),children(_Tree.children){
    _Tree.children.clear();
}

//destructor
Tree::~Tree() {
    clear();
}

//copy assignment operator
Tree & Tree::operator=(const Tree &_Tree) {
    if (this!=&_Tree){
        clear();
        node = _Tree.node;
        for(Tree* child : _Tree.children){
            addChild(*child);
        }
    }
    return *this;
}

//move assignment operator
Tree & Tree::operator=(Tree&& _Tree) {
    if (this!=&_Tree){
        clear();
        node = _Tree.node;
        children=_Tree.children;
        _Tree.children.clear();
    }
    return *this;
}

void Tree::clear() {
    for(Tree* child : children){
        if (child){
            delete child;
        }
    }
    children.clear();
}
//Tree methods
Tree* Tree::createTree(const Session& session, int rootLabel) {
    const int& treeType = session.getTreeType();
    if (treeType==0){
        return (new CycleTree(rootLabel,session.getCurrCycle()));
    }
    else if (treeType==1){
        return (new MaxRankTree(rootLabel));
    }
    else{
        return (new RootTree(rootLabel));
    }
}

void Tree::addChild(const Tree& child) {
    children.push_back(child.clone());
}

void Tree::addChild(Tree* child) {
    children.push_back(child);
}

Tree * CycleTree::clone() const {
    return new CycleTree(*this);
}

Tree * MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}

Tree * RootTree::clone() const {
    return new RootTree(*this);
}
int Tree::getRoot() const {return node;}

int Tree::getChildrenNum() const{return children.size();}

const Tree& Tree::getChild(int nodeInd) const{return *children[nodeInd];}

//CycleTree counstructor
CycleTree::CycleTree(int rootLabel, int currCycle):Tree(rootLabel),currCycle(currCycle){}

int CycleTree::traceTree() {
    const Tree* curr = this;
    int cycles = currCycle;
    while (cycles>0){
        if (curr->getChildrenNum()==0)
            return curr->getRoot();
        else{
            cycles--;
            curr = &(curr->getChild(0));
        }
    }
    return curr->getRoot();
}

//MaxRankTree constructor
MaxRankTree::MaxRankTree(int rootLabel)
    :Tree(rootLabel) {}

int MaxRankTree::traceTree() {
    Tree *root = this;
    int output = root->getRoot();
    int maxRank =  getChildrenNum();
    queue<const Tree *> Q = queue<const Tree *>();
    Q.push(root);
    while (!Q.empty()) {
        const Tree *u = Q.front();
        Q.pop();
        for (int i = 0; i < u->getChildrenNum(); ++i) {
            const Tree *nextNeighbor = &u->getChild(i);
            if (maxRank < nextNeighbor->getChildrenNum()) {
                maxRank = nextNeighbor->getChildrenNum();
                output = nextNeighbor->getRoot();
            }
            Q.push(nextNeighbor);
        }
    }
    return output;
}

//RootTree constructor
RootTree::RootTree(int rootLabel):Tree(rootLabel) {}

int RootTree::traceTree() {return getRoot();}