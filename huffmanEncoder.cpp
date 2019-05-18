#include "adaptiveHuffman_Encoder.h"

int main(){
  // set up Encoder class object
  HuffmanEncoder* enc = new HuffmanEncoder();

  // Take user input for file names
  std::string inFileName;
  std::string outFileName;

  // Prompt for input file name
  std::cout << "Please specify name of the input file:\t";
  std::cin >> inFileName;

  // Feed input name to encoder object
  enc -> setInputFile(inFileName);

  // Check if file is readable
  while(!enc -> inStreamOpen()){
    // If not, prompt for new file
    std::cout << "Target input file not readable.\n";
    std::cout << "Please specify another name of an input file:\t";
    std::cin >> inFileName;

    // Feed input name to encoder object
    enc -> setInputFile(inFileName);
  }

  // Prompt for output file name
  std::cout << "Please specify name of the output file:\t";
  std::cin >> outFileName;

  // Feed output name to encoder object
  enc -> setOutputFile(outFileName);

  // Check if file is readable
  while(!enc -> outStreamOpen()){
    // If not, prompt for new file
    std::cout << "Target input file not writable.\n";
    std::cout << "Please specify another name of an output file:\t";
    std::cin >> outFileName;

    // Feed input name to encoder object
    enc -> setOutputFile(outFileName);
  }


  // Begin encoding
  enc -> beginEncoding();

  // Delete the encoder object
  delete enc;
  return 0;
}
