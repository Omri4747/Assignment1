#include "../include/Tree.h"


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
    for (int i = 0; i < (int) children.size(); ++i) {
        _Tree.children[i]=nullptr;
    }
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
const Tree & Tree::operator=(Tree&& _Tree) {
    if (this!=&_Tree){
        clear();
        node = _Tree.node;
        children=_Tree.children;
        for (int i = 0; i < (int) _Tree.children.size(); ++i) {
            _Tree.children[i]= nullptr;
        }
    }
    return *this;
}

void Tree::clear() {
    for(Tree* child : children){
        if (child) delete child;
    }
}
//Tree methods
Tree* Tree::createTree(const Session& session, int rootLabel) {
    int treeType = session.getTreeType();
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
    return new CycleTree(getRoot(),currCycle);
}

Tree * MaxRankTree::clone() const {
    return new MaxRankTree(getRoot());
}

Tree * RootTree::clone() const {
    return new RootTree(getRoot());
}
int Tree::getRoot() const {return node;}

vector<Tree*> Tree::getChildren() const {return children;}

//CycleTree counstructor
CycleTree::CycleTree(int rootLabel, int currCycle):Tree(rootLabel),currCycle(currCycle){}

int CycleTree::traceTree() {
    Tree* curr = this;
    int cycles = currCycle;
    while (cycles>0){
        if ((int) curr->getChildren().size()==0)
            return curr->getRoot();
        else{
            cycles--;
            curr = curr->getChildren()[0];
        }
    }
    return curr->getRoot();
}

//MaxRankTree constructor
MaxRankTree::MaxRankTree(int rootLabel)
    :Tree(rootLabel) {}

int MaxRankTree::traceTree() {
    Tree* root = this;
    map<int,int> ranksMap ={};
    map<int,int> depthMap ={};
    depthMap.insert({getRoot(),0});
    ranksMap.insert({getRoot(),0});
    queue<Tree*> Q = queue<Tree*>();
    Q.push(root);
    while (!Q.empty()){
        Tree* u = Q.front();
        Q.pop();
        for (int i = 0; i < (int) u->getChildren().size(); ++i) {
                ranksMap[u->getRoot()]++;
                Tree* nextNeighbor = u->getChildren()[i];
                depthMap.insert({nextNeighbor->getRoot(),depthMap[u->getRoot()]+1});
                ranksMap.insert({nextNeighbor->getRoot(),0});
                Q.push(nextNeighbor);
        }
    }
    map<int,int>::iterator itr;
    int output = root->getRoot();
    for (itr = ranksMap.begin(); itr != ranksMap.end() ; itr++) {
        if (ranksMap[output] < ranksMap[itr->first]) {  //itr->first has more children than output
            output = itr->first;
        }
        else if(ranksMap[output] == ranksMap[itr->first]) {   //itr->first and output has same num of children
            if (depthMap[output] > depthMap[itr->first]) { //itr-first is closer to the root
                output = itr->first;
            } else if (depthMap[output] == depthMap[itr->first]) {    //output and itr-first are in the same depth
                if (output > itr->first)    //checks if output is bigger or smaller (smaller is most left child)
                    output = itr->first;
            }
        }
    }
    return output;
}

//RootTree constructor
RootTree::RootTree(int rootLabel)
    :Tree(rootLabel) {}

int RootTree::traceTree() {return getRoot();}