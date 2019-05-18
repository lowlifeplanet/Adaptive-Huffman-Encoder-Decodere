#ifndef ADAPTIVE_HUFFMAN_ENCODER_H
#define ADAPTIVE_HUFFMAN_ENCODER_H

#include "adaptiveHuffman_Tree.h"

#include <fstream>

class HuffmanEncoder{
  public:
    HuffmanEncoder();
    ~HuffmanEncoder();

    void beginEncoding();

    void setInputFile(std::string in);
    bool inStreamOpen();

    void setOutputFile(std::string out);
    bool outStreamOpen();
  private:
    HuffmanTree* tree;

    std::string inFileName;
    std::string outFileName;

    std::ifstream inFile;
    std::ofstream outFile;

    void encode(int symbol);

    std::string getEncoding(int symbol);
};

#endif
