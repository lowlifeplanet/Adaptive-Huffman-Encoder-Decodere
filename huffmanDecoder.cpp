#include "adaptiveHuffman_Decoder.h"

int main(){
  // set up Decoder class object
  HuffmanDecoder* dec = new HuffmanDecoder();

  // Take user input for file names
  std::string inFileName;
  std::string outFileName;

  // Prompt for input file name
  std::cout << "Please specify name of the input file:\t";
  std::cin >> inFileName;

  // Feed input name to Decoder object
  dec -> setInputFile(inFileName);

  // Check if file is readable
  while(!dec -> inStreamOpen()){
    // If not, prompt for new file
    std::cout << "Target input file not readable.\n";
    std::cout << "Please specify another name of an input file:\t";
    std::cin >> inFileName;

    // Feed input name to Decoder object
    dec -> setInputFile(inFileName);
  }

  // Prompt for output file name
  std::cout << "Please specify name of the output file:\t";
  std::cin >> outFileName;

  // Feed output name to Decoder object
  dec -> setOutputFile(outFileName);

  // Check if file is readable
  while(!dec -> outStreamOpen()){
    // If not, prompt for new file
    std::cout << "Target input file not writable.\n";
    std::cout << "Please specify another name of an output file:\t";
    std::cin >> outFileName;

    // Feed input name to Decoder object
    dec -> setOutputFile(outFileName);
  }


  // Begin decoding
  dec -> beginDecoding();

  // Delete the Decoder object
  delete dec;
  return 0;
}
