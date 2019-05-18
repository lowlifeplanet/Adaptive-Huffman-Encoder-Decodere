#include "adaptiveHuffman_Tree.h"

#include <stdio.h>
#include <iostream>

int main(){
  std::cout << "Creating tree...\n";
  HuffmanTree myTree = HuffmanTree();

  int in;
  std::cout << "Taking input: \t";
  while(std::cin >> in){
    myTree.addSymbol(in);
    myTree.printAllNodes();

    std::cout << "Taking input: \t";
  }

  return 0;
}
