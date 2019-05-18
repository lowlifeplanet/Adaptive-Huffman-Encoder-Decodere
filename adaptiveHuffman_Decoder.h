#ifndef ADAPTIVE_HUFFMAN_DECODER_H
#define ADAPTIVE_HUFFMAN_DECODER_H

#include "adaptiveHuffman_Tree.h"

#include <fstream>

class HuffmanDecoder{
  public:
      HuffmanDecoder();
      ~HuffmanDecoder();

      void beginDecoding();

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

    bool newLine;

    void record(int symbol);
    int decodeInitial(std::string code);
};

#endif
