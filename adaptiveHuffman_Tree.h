#ifndef ADAPTIVE_HUFFMAN_TREE_H
#define ADAPTIVE_HUFFMAN_TREE_H

#include <map>
#include <string>
#include <cstddef>  // for NULL definition
#include <bitset>   // for easier bitwise operations/conversions
#include <iostream>

#include "adaptiveHuffman_Node.h"

class HuffmanTree{
  public:
    HuffmanTree();
    ~HuffmanTree();

    void addSymbol(int newSymbol);

    Node* getRoot();
    Node* getNYT();
    Node* getNode(short targetImp);
    Node* getSymbolNode(int symbol);

    void printAllNodes();

    //Block* getBlock(double targetWeight);

    std::string getInitialCode(int symbol);
    bool isEncoded(int symbol);

  private:

    Node* NYT;    // NYT node reference, for quick access during encoding
    int nodeCount;  // Number of nodes currently created

    std::map<int, Node*> directory;   // Directory of values to node references
    std::map<int, std::string> initDir;    // Initial directory of values to strings

    double blockWgt[1025];  // List of block weights for each implicit number
    Node* allNodes[1025]; // All nodes, with relation to their implicit numbers
    // Since alphabet contains 512 unique values (-255 ~ 255 and line break),
    // (512 * 2) + 1 = 1025 possible nodes in worst case scenario

    void Definitions_Init();

    void slideAndIncrement(Node* p);

    void block_addNode(short implicit, Node* newNode, double weight, bool leaf);
    short block_getLeader(double blockNum);
    short block_getHighest(double blockNum);
    bool block_isEmpty(double blockNum);

};

#endif
