#include "adaptiveHuffman_Encoder.h"

// constrictor
// Mostly constructs
HuffmanEncoder::HuffmanEncoder(){
  tree = new HuffmanTree();
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// el destructeur
HuffmanEncoder::~HuffmanEncoder(){
  delete tree;
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Set new input file name and open filestream
void HuffmanEncoder::setInputFile(std::string in){
  // Set file name
  inFileName = in;

  // Attempt to open file stream
  inFile.open(inFileName);
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Set new output file name and open filestream
void HuffmanEncoder::setOutputFile(std::string out){
  // Set file name
  outFileName = out;

  // Attempt to open file stream
  outFile.open(outFileName);
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Check if input stream is open
bool HuffmanEncoder::inStreamOpen(){
  return inFile.is_open();
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Check if input stream is open
bool HuffmanEncoder::outStreamOpen(){
  return outFile.is_open();
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Full encoding procedure
void HuffmanEncoder::beginEncoding(){
  std::cout << "Beginning encoding...\n";
  std::string readBuffer;
  char c;

  // Grab single character at a time, until end of file
  while(inFile.get(c)){
    if(c == ','){
      // Encode the last few numbers read
      encode(std::stoi(readBuffer));
      // Clear read buffer;
      readBuffer = "\0";
    }else if(c == '\n'){
      // Encode the last few numbers read
      encode(std::stoi(readBuffer));
      // Clear read buffer;
      readBuffer = "\0";

      // Encode \n as -256
      encode(-256);
    }else{
      // Save character in buffer
      readBuffer = readBuffer + c;
    }
  }

  if(inFile.is_open()){
    inFile.close();
  }

  if(outFile.is_open()){
    outFile.close();
  }
  std::cout << "Encoding finished!\n";
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Encoding for each symbol
void HuffmanEncoder::encode(int symbol){
  // Set aside buffer for file write
  std::string buffer;

  // Check if symbol is already encoded
  if(tree -> isEncoded(symbol)){
    // If already encoded, simply get the tree encoding of the symbol
    buffer = getEncoding(symbol);
  }else{
    // If not yet encoded, send NYT, then send initial code for symbol to output
    buffer = getEncoding(500) + tree ->getInitialCode(symbol);
  }

  // Write encoded data to output file
  outFile << buffer;

  // Add symbol to tree
  tree ->addSymbol(symbol);
}

// ==== THE END IS NEVER THE END IS NEVER THE END IS NEVER THE END IS NEVER ====

// Retrieve encoding in tree
std::string HuffmanEncoder::getEncoding(int symbol){
  // Find target node location
  Node* n = NULL;

  // Check for NYT symbol input
  if(symbol == 500){
    n = tree ->getNYT();
  }else{
    n = tree ->getSymbolNode(symbol);
  }

  // Set aside buffer for return
  std::string symbolBuffer;

  // Check if node exists
  // Should always exist when this function is called
  if(n != NULL){
    // Iterate while node has a parent
    while((n -> getParent()) >= 0){
      if(n -> getParentBit()){
        symbolBuffer = "1" + symbolBuffer;
      }else{
        symbolBuffer = "0" + symbolBuffer;
      }

      // Change n to its parent
      n = tree ->getNode(n -> getParent());
    }
  }

  return symbolBuffer;
}
