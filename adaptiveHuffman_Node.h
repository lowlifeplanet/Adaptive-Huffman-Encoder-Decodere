#ifndef ADAPTIVE_HUFFMAN_NODE_H
#define ADAPTIVE_HUFFMAN_NODE_H

class Node{
  public:

    Node();
    Node(int insert_value);
    Node(int insert_value, short implicit);

    void editChild(bool bit, short childImp);
    short getChild(bool bit);
    short getParent();
    bool getParentBit();
    void setParent(short parentImp, bool bit);
    int getValue();
    unsigned int getWeight();
    void setWeight(unsigned int newWeight);
    bool isLeaf();
    void setLeaf(bool leaf);

    void setImplicit(short num);
    short getImplicit();

    ~Node();

  private:
    int value;              // Stored value
    unsigned int weight;    // Weight of node
    bool leafNode;          // Signals internal or leaf node
    short impNum;    // Implicit number of node, as defined by Vitter

    short parent;    // Parent implicit number
    bool pBit;              // Signals 0 or 1 edge from parent
    short child[2];  // Children node implicit numbers
};

#endif
