#include "adaptiveHuffman_Decoder.h"

#include <thread> // for sleep_for
#include <chrono>

// Mostly constructs
HuffmanDecoder::HuffmanDecoder(){
  tree = new HuffmanTree();
  newLine = true;
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

// el destructeur
HuffmanDecoder::~HuffmanDecoder(){
  delete tree;
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

// Set new input file name and open filestream
void HuffmanDecoder::setInputFile(std::string in){
  // Set file name
  inFileName = in;

  // Attempt to open file stream
  inFile.open(inFileName, std::ifstream::in);
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

// Set new output file name and open filestream
void HuffmanDecoder::setOutputFile(std::string out){
  // Set file name
  outFileName = out;

  // Attempt to open file stream
  outFile.open(outFileName, std::ifstream::out);
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

// Check if input stream is open
bool HuffmanDecoder::inStreamOpen(){
  return inFile.is_open();
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

// Check if input stream is open
bool HuffmanDecoder::outStreamOpen(){
  return outFile.is_open();
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

void HuffmanDecoder::beginDecoding(){
  std::cout << "Beginning decoding...\n";
  Node* targetNode = tree -> getRoot();
  std::string initCodeBuffer;

  char c;

  // Decode the first fixed dictionary entry
  for(int i = 0; i < 9; i++){
    inFile.get(c);
    initCodeBuffer = initCodeBuffer + c;
  }

  // Record the newly decoded symbol & add to tree
  record(decodeInitial(initCodeBuffer));

  // Reset init code buffer
  initCodeBuffer = "\0";

  // Continue to grab characters until end of file
  while(inFile.get(c)){
    // Follow encoding from node to a child
    if(c == '0'){
      targetNode = tree -> getNode(targetNode -> getChild(0));
    }else{
      targetNode = tree -> getNode(targetNode -> getChild(1));
    }

    // Check if a leaf is reached
    if(targetNode -> isLeaf()){
      // If leaf is reached, grab its value
      int val = targetNode -> getValue();

      // Check if val = NYT
      if(val == 500){
        // If value is NYT, decode the next fixed code
        for(int i = 0; i < 9; i++){
          inFile.get(c);
          initCodeBuffer = initCodeBuffer + c;
        }

        val = decodeInitial(initCodeBuffer);

        // Reset init code buffer
        initCodeBuffer = "\0";
      }
      // Record the newly decoded symbol & add to tree
      record(val);

      // Reset position back to root
      targetNode = tree -> getRoot();
    }
  }

  std::cout << "Decoding complete!\n";

  if(inFile.is_open()){
    inFile.close();
  }

  if(outFile.is_open()){
    outFile.close();
  }
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

int HuffmanDecoder::decodeInitial(std::string code){
  int symbol = 0;

  // If first character is 1, then number is negative, must take 2's complement
  if(code.at(0) == '1'){
    for(int i = 0; i < (code.length() - 1); i++){
      if(code.at(code.length() - i - 1) == '0'){
        symbol += (1 << i);
      }
    }
    // add 1 to result then flip negative/positive
    symbol = (symbol + 1) * (-1);
  }else{
    // If first character is 0, then number is positive.
    for(int i = 0; i < (code.length() - 1); i++){
      if(code.at(code.length() - i - 1) == '1'){
        symbol += (1 << i);
      }
    }
  }

  return symbol;
}

// ============= THE INNER MACHINATIONS OF MY MIND ARE AN ENIGMA ===============

// Record the decoded symbol and add the symbol to the tree
void HuffmanDecoder::record(int symbol){
  // Print comma delimitor between items of the same line
  if((!newLine) && (symbol != -256)){
    outFile << ",";
  }

  // Check if decoded symbol is a line break
  if(symbol == -256){
    // If it is, print out a line break and set newLine to true
    outFile << "\n";
    newLine = true;
  }else{
    // print out the decoded symbol
    outFile << symbol;
    newLine = false;
  }

  // Add the new symbol to the tree
  tree -> addSymbol(symbol);
}
