#include "adaptiveHuffman_Node.h"

// Constructor
Node::Node(){
  value = -511; // Null value placeholder
  parent = -1;
  pBit = 0;
  impNum = -1;
  weight = 0;
  leafNode = true;
  child[0] = -1;
  child[1] = -1;
}

//============================ PARTING OF THE Cs ===============================

// Constructor
Node::Node(int insert_value){
  value = insert_value;

  // -1 represents non-existent parent/child node
  parent = -1;
  pBit = 0;

  impNum = -1;
  weight = 0;
  leafNode = true;

  child[0] = -1;
  child[1] = -1;
}

//============================ PARTING OF THE Cs ===============================

// Constructor
Node::Node(int insert_value, short implicit){
  value = insert_value;
  impNum = implicit;

  // -1 represents non-existent parent/child node
  parent = -1;
  pBit = 0;

  weight = 0;
  leafNode = true;

  child[0] = -1;
  child[1] = -1;
}

//============================ PARTING OF THE Cs ===============================

// Add a child node, regardless of one existed or not
void Node::editChild(bool bit, short childImp){
    child[bit] = childImp;
    // Internal node never reverts to leaf node, thus do not need to check
    // for condition to revert
    leafNode = false;
}

//============================ PARTING OF THE Cs ===============================

// Returns implicit number of child at 0 or 1 connection
short Node::getChild(bool bit){
  return child[bit];
}

//============================ PARTING OF THE Cs ===============================

// Returns parent implicit number
short Node::getParent(){
  return parent;
}

//============================ PARTING OF THE Cs ===============================

// Returns 0 or 1 connection to parent
bool Node::getParentBit(){
  return pBit;
}

//============================ PARTING OF THE Cs ===============================

// Sets a new parent node and parent connection bit
void Node::setParent(short parentImp, bool bit){
  parent = parentImp;
  pBit = bit;
}

//============================ PARTING OF THE Cs ===============================

// Returns stored value of the node
int Node::getValue(){
  return value;
}

//============================ PARTING OF THE Cs ===============================

// Returns weight value
unsigned int Node::getWeight(){
  return weight;
}

//============================ PARTING OF THE Cs ===============================

// set a new weight
void Node::setWeight(unsigned int newWeight){
  weight = newWeight;
}

//============================ PARTING OF THE Cs ===============================

// Returns whether or not node is a leaf
bool Node::isLeaf(){
  return leafNode;
}

//============================ PARTING OF THE Cs ===============================

// set whether or not node is a leaf.
// May not be useful since editChild already disables leaf eligibility.
void Node::setLeaf(bool leaf){
  leafNode = leaf;
}

//============================ PARTING OF THE Cs ===============================

// Set implicit number
void Node::setImplicit(short num){
  impNum = num;
}

//============================ PARTING OF THE Cs ===============================

// Get implicit number
short Node::getImplicit(){
  return impNum;
}

//============================ PARTING OF THE Cs ===============================

// Destructor
Node::~Node(){
  // I don't know why we still have this, to be honest.
}
