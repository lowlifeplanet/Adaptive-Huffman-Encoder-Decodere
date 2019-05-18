#include "adaptiveHuffman_Tree.h"

#include <stdio.h>
#include <thread> // for sleep_for
#include <chrono>
#include <algorithm>
#include <math.h>

// Constructor
HuffmanTree::HuffmanTree(){
  // Initialize starting directory
  Definitions_Init();

  // Unlike the definition of Vitter, we will use lowest number at root
  // and have numbers increment as depth increases

  // Create root node, also acts as inital NYT node, implicit number 0.
  allNodes[0] = new Node(500, 0);
  NYT = allNodes[0];

  // Initialize collection array of all nodes
  for(int i = 1; i < 1025; i++){
    allNodes[i] = NULL;
  }

  nodeCount = 1;

}

//============================ I AM A DIVIDER LINE ============================

// Initializes starting directory
void HuffmanTree::Definitions_Init(){
  // Encode each of the possible color/difference values, from -255 to 255
  // Also includes -256 to represent line change symbol '\n'
  for(int i = -256; i < 256; i++){
    // Convert signed integer to signed 9-bit binary representation string
    // Then add newly generated entry to initial directory
    initDir[i] = std::bitset<9>(i).to_string();
  }

  // Encode single 0 for initial NYT entry
  // Use 500 to represent NYT
  initDir[500] = "0";
}

//============================ I AM A DIVIDER LINE ============================

// Destructor
HuffmanTree::~HuffmanTree(){

  // Delete all created nodes
  for(int i = 0; i < nodeCount; i++){
    delete allNodes[i];
  }
}

//============================ I AM A DIVIDER LINE ============================

// Add symbol function, as specified by Vitter algorithm
void HuffmanTree::addSymbol(int newSymbol){
  Node* p;
  if(directory.find(newSymbol) == directory.end()){
    // Case where symbol has not yet been transmitted

    // Create 2 children nodes
    // Create right node first, with new symbol
    Node* rightChild = new Node(newSymbol, nodeCount);
    allNodes[nodeCount] = rightChild;
    nodeCount++;

    // Create left node, with value NYT
    Node* leftChild = new Node(500, nodeCount);
    allNodes[nodeCount] = leftChild;
    nodeCount++;

    // Add children to original NYT node
    // Add right child
    NYT -> editChild(1, rightChild -> getImplicit());
    rightChild -> setParent((NYT -> getImplicit()), 1);

    // Add left child
    NYT -> editChild(0, leftChild -> getImplicit());
    leftChild -> setParent((NYT -> getImplicit()), 0);

    // Set necessary update values
    p = rightChild;

    // Update new NYT location
    NYT = leftChild;

    // Insert directory entry for newly transmitted symbol
    directory[newSymbol] = rightChild;

  }else{
    // Case where symbol has been transmitted

    // set p according to directory entry
    p = directory[newSymbol];
  }

  // Update tree with slideAndIncrement
  slideAndIncrement(p);
}

//============================ I AM A DIVIDER LINE ============================

// Returns root node address
Node* HuffmanTree::getRoot(){
  // Root node will always have implicit number 0
  return allNodes[0];
}

//============================ I AM A DIVIDER LINE ============================

// Returns address of NYT node
Node* HuffmanTree::getNYT(){
    return NYT;
}

//============================ I AM A DIVIDER LINE ============================

// Retrieve the node address from a specific implicit number
// Used for child/parent access
Node* HuffmanTree::getNode(short targetImp){
    return allNodes[targetImp];
}

//============================ I AM A DIVIDER LINE ============================

// Slide & increment routine for all nodes
void HuffmanTree::slideAndIncrement(Node* p){
  //std::cout << "Checkpoint 1: p = " << p << ", pIN = " << p -> getImplicit() << "\n";
  // Grab current node's implicit number & weight
  short pNum = p -> getImplicit();
  unsigned int pWgt = p -> getWeight();
  bool leaf = p -> isLeaf();

  Node* prevP = NULL;

  if(!leaf){
    if(p -> getParent() >= 0){
      prevP = allNodes[p -> getParent()];
    }
  }

  //std::cout << "Checkpoint 2\n";
  // Define starting block for move
  double startBlock = pWgt + (0.5 * (!leaf));

  // Check if n is a leaf or internal node
  //std::cout << "Checkpoint 3, startBlock = " << startBlock << "\n";

  if(leaf){
    // Leaf: move before update

    // Check if weight is at least 1
    if(pWgt > 0){
      short targetNumber = pNum;
      //std::cout << "Checkpoint 3.1\n";
      // Find the leader of the block in which p belongs
      short oldBlockLeader = block_getLeader(startBlock);

      //std::cout << "Checkpoint 3.2: startBlock = " << startBlock << ", block empty = " << block_isEmpty(startBlock) << "\n";
      // Remove the leader implicit number from the block member list
      // blockChain[startBlock] -> removeLowest();

      //std::cout << "Checkpoint 3.3: pNum = " << pNum << ", oldBlockLeader = " << oldBlockLeader <<"\n";
      // Shift all entries in the block member list up by 1 spot,
      // starting from the implicit number of p until the old leader
      for(int i = pNum; i > oldBlockLeader; i--){
        // Update directory
        allNodes[i] = allNodes[i-1];

        // Update node internal information
        allNodes[i] -> setImplicit(i);

        if(!(allNodes[i] -> isLeaf())){
          // Update node children's parent implicit number information
          allNodes[allNodes[i] -> getChild(0)] -> setParent(i, 0);
          allNodes[allNodes[i] -> getChild(1)] -> setParent(i, 1);
        }
        // Add the entry to the corresponding block
        // blockChain[(allNodes[i] -> getWeight()) + (0.5 * (!(allNodes[i] -> isLeaf())))] -> addNode(i, allNodes[i]);
        block_addNode(i, allNodes[i], allNodes[i] -> getWeight(), allNodes[i] -> isLeaf());

      }

      targetNumber = oldBlockLeader;

      //std::cout << "Checkpoint 3.5: NYT = " << NYT << ", parent = "<< NYT -> getParent() <<"\n";
      // Check if the block leader's position is sibling to NYT
      if(oldBlockLeader != (allNodes[NYT -> getParent()] -> getChild(!(NYT -> getParentBit())))){
        // If not sibling to NYT, then node can potentially move further

        //std::cout << "Checkpoint 3.5.1\n";
        // Calculate index of next block
        double midBlock = startBlock + 0.5;

        //std::cout << "Checkpoint 3.5.2\n";
        // Check for existence of nodes in the next block
        if(!(block_isEmpty(midBlock))){
          // If at least 1 node exists, then p can move further
          short midBlockLeader = block_getLeader(midBlock);

          // Remove the leader implicit number from the member list
          // blockChain[midBlock] -> removeLowest();

          // Shift all entries in the block member list up by 1 spot,
          // starting from the leader of the old block until the current leader

          //std::cout << "Checkpoint 3.5.3 : oldBlockLeader = " << oldBlockLeader << ", midBlockLeader = " << midBlockLeader <<"\n";
          for(int i = oldBlockLeader; i > midBlockLeader; i--){
            //std::cout << "Checkpoint 3.5.3.1: i = " << i << ", allNodes[i] = " << allNodes[i] << ", allNodes[i-1] = " << allNodes[i-1] <<" \n";
            // Update directory
            allNodes[i] = allNodes[i-1];

            //std::cout << "Checkpoint 3.5.3.2\n";
            // Update node internal information
            allNodes[i] -> setImplicit(i);

            //std::cout << "Checkpoint 3.5.3.3\n";

            if(!(allNodes[i] -> isLeaf())){
              //std::cout << "Checkpoint 3.5.3.3.1\n";
              // Update node children's parent implicit number information
              allNodes[allNodes[i] -> getChild(0)] -> setParent(i, 0);
              allNodes[allNodes[i] -> getChild(1)] -> setParent(i, 1);
            }

            //std::cout << "Checkpoint 3.5.3.4\n";
            // Add the entry to the corresponding block
            // blockChain[(allNodes[i] -> getWeight()) + (0.5 * (!(allNodes[i] -> isLeaf())))] -> addNode(i, allNodes[i]);
            block_addNode(i, allNodes[i], allNodes[i] -> getWeight(), allNodes[i] -> isLeaf());
          }


          //std::cout << "Checkpoint 3.5.4: midBlock = " << midBlock << ", oldBlockLeader = " << oldBlockLeader << "\n";

          if(oldBlockLeader > midBlockLeader){
            // Add the node in the previous block's leader's implicit number spot
            // to the current block (to signify the shift)
            // blockChain[midBlock] -> addNode(oldBlockLeader, allNodes[oldBlockLeader]);
            block_addNode(oldBlockLeader, allNodes[oldBlockLeader], allNodes[oldBlockLeader] -> getWeight(), allNodes[oldBlockLeader] -> isLeaf());

            // Change shift target location to mid block leader's location
            targetNumber = midBlockLeader;
          }
        }
        //std::cout << "Checkpoint 3.5.5\n";
        // If no nodes in next block, simply skip to the next step
      }
      // If leader is sibling to NYT, then there was no movement at all, while
      // symbol is determined to be THE SINGLE least occuring in the program
      // No chance for further movement without breaking algorithm.
      // Example: input ABCACB, the last B input,
      // where B node will not be able to move anywhere.
      //std::cout << "Checkpoint 3.6\n";

      // Add p to the front of the shift location
      allNodes[targetNumber] = p;
      p -> setImplicit(targetNumber);

    }
    // If weight started at 0,
    // No need to move since 0 will always be
    // to the right of NYT

    //std::cout << "Checkpoint 4\n";

  }
  // Weight update routine
  // Increase leaf weight by 1
  p -> setWeight((p -> getWeight()) + 1);

  //std::cout << "Checkpoint 5\n";

  if(!leaf){
    // Internal: update before move
    //std::cout << "Checkpoint 5.1\n";
    short targetNumber = pNum;
    bool atRoot = (pNum == 0);
    short oldBlockLeader = -1;

    //std::cout << "Checkpoint 5.2\n";
    // Check if node was in a block of at least weight 1
    if(pWgt > 0){
      //std::cout << "Checkpoint 5.2.1, startBlock = " << startBlock << "\n";
      // If node started with at least weight 1,
      double prevBlock = pWgt + 0.0;

      // Grab value of old block leader
      oldBlockLeader = block_getLeader(startBlock);

      // Shift all internal nodes in old block

      //std::cout << "Checkpoint 5.2.2: startBlock = " << startBlock << ", block empty = " << block_isEmpty(startBlock) << "\n";
      // Remove the leader implicit number from the block
      // blockChain[startBlock] -> removeLowest();



      //std::cout << "Checkpoint 5.2.3\n";

      // Check if working with root

      // Shift all nodes' implicit numbers up by 1
      //std::cout << "Checkpoint 5.2.4: pNum = " << pNum << ", oldBlockLeader = " << oldBlockLeader <<"\n";
      for(int i = pNum; i > oldBlockLeader; i--){
        // Update directory
        allNodes[i] = allNodes[i-1];

        // Update node internal information
        allNodes[i] -> setImplicit(i);

        if(!(allNodes[i] -> isLeaf())){
          // Update node children's parent implicit number information
          allNodes[allNodes[i] -> getChild(0)] -> setParent(i, 0);
          allNodes[allNodes[i] -> getChild(1)] -> setParent(i, 1);
        }

        //std::cout << "Checkpoint 5.2.4.1\n";
        // Check for existence of corrensponding block
        double bWgt = (allNodes[i] -> getWeight()) + (0.5 * (!(allNodes[i] -> isLeaf())));
/*
        if(blockChain.find(bWgt) == blockChain.end()){
          blockChain[bWgt] = new Block(floor(bWgt) ,(allNodes[i] -> isLeaf()));
        }
        */
        // Add the entry to the corresponding block
        // blockChain[bWgt] -> addNode(i, allNodes[i]);
        block_addNode(i, allNodes[i], allNodes[i] -> getWeight(), allNodes[i] -> isLeaf());
      }

      //std::cout << "Checkpoint 5.2.5\n";
    }else{
      // If the original weight was 0, do only the second shift phase
      oldBlockLeader = pNum;
    }
    // If node started at weight 0, no need to shift from start block
    // since weight 0 internal node block should not exist.

    // Change target location to the leader position
    targetNumber = std::min(targetNumber, oldBlockLeader);

    //std::cout << "Checkpoint 5.3\n";
    // Calculate index of next block
    double midBlock = startBlock + 0.5;

    //std::cout << "Checkpoint 5.4\n";
    // Check for existence of the next block
    //if(blockChain.find(midBlock) != blockChain.end()){
      //std::cout << "Checkpoint 5.4.1\n";
      // Check for existence of nodes in the next block
      if(!(block_isEmpty(midBlock))){
        //std::cout << "Checkpoint 5.4.1.1: midBlock = " << midBlock << "\n";
        // If at least 1 node exists, then p can potentially move further
        short midBlockLeader = block_getLeader(midBlock);
        // Check if leader node is siblings with NYT
        if((allNodes[NYT -> getParent()] -> getChild(!(NYT -> getParentBit()))) != midBlockLeader){
          // Only operate if sibling is not NYT
          //std::cout << "Checkpoint 5.4.1.2: midBlockLeader = " << midBlockLeader << "\n";
          // Remove the leader implicit number from the member list
          // blockChain[midBlock] -> removeLowest();

          //std::cout << "Checkpoint 5.4.1.3\n";
          // Shift all entries in the block member list up by 1 spot,
          // starting from the leader of the old block until the current leader
          //std::cout << "Checkpoint 5.4.1.4: oldBlockLeader = " << oldBlockLeader << ", midBlockLeader = " << midBlockLeader <<"\n";

          for(int i = oldBlockLeader; i > midBlockLeader; i--){
            //std::cout << "Checkpoint 5.4.1.4.1\n";
            // Update directory
            allNodes[i] = allNodes[i-1];

            //std::cout << "Checkpoint 5.4.1.4.2\n";
            // Update node internal information
            allNodes[i] -> setImplicit(i);

            //std::cout << "Checkpoint 5.4.1.4.3\n";

            // Check if node is internal
            // This should never happen
            if(!(allNodes[i] -> isLeaf())){
              //std::cout << "Checkpoint 5.4.1.4.3.5\n";

              // Update node children's parent implicit number information
              allNodes[allNodes[i] -> getChild(0)] -> setParent(i, 0);
              allNodes[allNodes[i] -> getChild(1)] -> setParent(i, 1);

            }
            //std::cout << "Checkpoint 5.4.1.4.4\n";
            // Add the entry to the corresponding block
            // blockChain[(allNodes[i] -> getWeight()) + (0.5 * (!(allNodes[i] -> isLeaf())))] -> addNode(i, allNodes[i]);
            block_addNode(i, allNodes[i], allNodes[i] -> getWeight(), allNodes[i] -> isLeaf());

            //std::cout << "Checkpoint 5.4.1.4.5\n";
          }

          //std::cout << "Checkpoint 5.4.1.5\n";


          if(oldBlockLeader > midBlockLeader){
            // Add the node in the previous block's leader's implicit number spot
            // to the current block (to signify the shift)
            // (assuming previous block existed)
            //std::cout << "Checkpoint 5.4.1.5.1: midBlock = " << midBlock << ", oldBlockLeader = " << oldBlockLeader <<" \n";
            if(pWgt > 0){
              // blockChain[midBlock] -> addNode(oldBlockLeader, allNodes[oldBlockLeader]);
              block_addNode(oldBlockLeader, allNodes[oldBlockLeader], allNodes[oldBlockLeader] -> getWeight(), allNodes[oldBlockLeader] -> isLeaf());
            }

            // Charge target location to the leader position
            targetNumber = std::min(targetNumber, midBlockLeader);
          }

        }
      }
      // If no nodes in next block, simply skip to the next step
      //std::cout << "Checkpoint 5.4.2\n";

    //}



    //std::cout << "Checkpoint 5.5\n";
    // If next block does not exist, simply skip to next step

    // Add p to the front of the shift location
    allNodes[targetNumber] = p;
    p -> setImplicit(targetNumber);

    // Update P's children's parent info
    allNodes[p -> getChild(0)] -> setParent(targetNumber, 0);
    allNodes[p -> getChild(1)] -> setParent(targetNumber, 1);
  }

  //std::cout << "Checkpoint 6\n";

  if(p -> getImplicit() != 0){

    double newBlockWeight = (p -> getWeight()) + (0.5 * !(p -> isLeaf()));

    //std::cout << "Checkpoint 7\n";

    /*
    // Check if the appropriate weighted block exists
    if(blockChain.find(newBlockWeight) == blockChain.end()){
      //std::cout << "Checkpoint 7.1\n";
      // If it doesn't exist, create it
      blockChain[newBlockWeight] = new Block(p -> getWeight(), p -> isLeaf());
    }
*/
    //std::cout << "Checkpoint 8: newBlockWeight = " << newBlockWeight << ", p weight = " << p -> getWeight() << ", p isLeaf = " << p->isLeaf() <<"\n";

    // Insert node into new appropriate weight block
    // blockChain[newBlockWeight] -> addNode(p -> getImplicit(), p);
    block_addNode(p -> getImplicit(), p, p -> getWeight(), p -> isLeaf());


    // Find child information regarding P and update P's parent information


  }
  for(int i = 0; i < nodeCount; i++){
    if(allNodes[i] -> getChild(0) == p -> getImplicit()){
      p -> setParent(i, 0);
      break;
    }

    if(allNodes[i] -> getChild(1) == p -> getImplicit()){
      p -> setParent(i, 1);
      break;
    }
  }

  // Find extra nodes whose parent info doesn't match the parents' children info
  for(int j = nodeCount -1; j > 0; j--){
    Node* n = allNodes[j];

    if((((allNodes[n -> getParent()] -> getChild(0)) != j) && ((allNodes[n -> getParent()] -> getChild(1)) != j))
     || (allNodes[n -> getParent()] -> getChild(n -> getParentBit()) != j)){
      for(int i = 0; i < nodeCount; i++){
        if(allNodes[i] -> getChild(0) == j){
          n -> setParent(i, 0);
          block_addNode(n -> getImplicit(), n, n -> getWeight(), n -> isLeaf());
          break;
        }

        if(allNodes[i] -> getChild(1) == j){
          n -> setParent(i, 1);
          block_addNode(n -> getImplicit(), n, n -> getWeight(), n -> isLeaf());
          break;
        }
      }
    }else if(allNodes[n -> getParent()] -> getChild(n -> getParentBit()) != j) {

    }
  }


  if(leaf){
      prevP = allNodes[p -> getParent()];
  }

  if(prevP != NULL){
    //printAllNodes();
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    slideAndIncrement(prevP);
  }

}

//============================ I AM A DIVIDER LINE ============================

// Print all node information
// Used for debugging
void HuffmanTree::printAllNodes(){
  std::cout << "\nContent Report:\n\n";
  std::cout << "Num\tLeaf\tParent\tBit\tChild0\tChild1\tValue\tWeight\tAddress\n";
  for(int i = 0; i < nodeCount; i++){
    Node* n = allNodes[i];
    std::cout << n -> getImplicit() << "\t" << n -> isLeaf() << "\t" << n -> getParent();
    std::cout << "\t" << n -> getParentBit() << "\t" << n -> getChild(0);
    std::cout << "\t" << n -> getChild(1) << "\t" << n -> getValue();
    std::cout << "\t" << n -> getWeight() << "\t" << n <<"\n";
  }

  std::cout << "\n\nEnd of Content Report\n\n";
}

//============================ I AM A DIVIDER LINE ============================
/*
Block* HuffmanTree::getBlock(double targetWeight){
  if(blockChain.find(targetWeight) != blockChain.end()){
    return blockChain[targetWeight];
  }else return NULL;
}
*/
//============================ I AM A DIVIDER LINE ============================

std::string HuffmanTree::getInitialCode(int symbol){
  return initDir[symbol];
}


Node* HuffmanTree::getSymbolNode(int symbol){
  if(directory.find(symbol) != directory.end()){
    return directory[symbol];
  }else{
    return NULL;
  }
}

//============================ I AM A DIVIDER LINE ============================

bool HuffmanTree::isEncoded(int symbol){
  return !(directory.find(symbol) == directory.end());
}


//======================== TO SPLIT OR NOT TO SPLIT ===========================

// Add a new node to the block
void HuffmanTree::block_addNode(short implicit, Node* newNode, double weight, bool leaf){
  // Check if node fits criteria
  if((newNode -> isLeaf() == leaf) && (newNode -> getWeight() == weight)){
    blockWgt[implicit] = weight + (0.5 * (!(leaf)));
  }else{
    // Debug message
    std::cout << "Error in Block::addNode: \n";
    std::cout << "\tnode " << newNode  << " (" << implicit << ") does not fit block criteria.\n";
    std::cout << "\tnode weight & leaf status:\t" << newNode -> getWeight() << ", " << newNode -> isLeaf() << "\n";
    std::cout << "\tblock weight & leaf status:\t" << weight << ", " << leaf << "\n";
  }
}

//======================== TO SPLIT OR NOT TO SPLIT ===========================

// Retrieve the leader of the block
short HuffmanTree::block_getLeader(double blockNum){
  for(short i = 0; i < nodeCount; i++){
    if(blockNum == blockWgt[i]) return i;
  }
  return 9999;
}

//======================== TO SPLIT OR NOT TO SPLIT ===========================

// Retrieve highest implicit number of the block
// Used for debugging purposes
short HuffmanTree::block_getHighest(double blockNum){
  for(short i = nodeCount; i >= 0; i--){
    if(blockNum == blockWgt[i]) return i;
  }
  return -1;
}

/*
//======================== TO SPLIT OR NOT TO SPLIT ===========================

// Remove the lowest implicit number of the block (the leader)
void block_removeLowest(){
  members.erase(members.begin() -> first);
}

//======================== TO SPLIT OR NOT TO SPLIT ===========================

// Remove the highest implicit number of the list
void Block::removeHighest(){
  members.erase(members.rbegin() -> first);
}

//======================== TO SPLIT OR NOT TO SPLIT ===========================

void Block::removeElement(short target){
  members.erase(target);
}
*/
//======================== TO SPLIT OR NOT TO SPLIT ===========================

// Checks whether or not the block has no members
bool HuffmanTree::block_isEmpty(double blockNum){
  for(int i = 0; i < nodeCount; i++){
    if(blockWgt[i] == blockNum){
      return false;
    }
  }
  return true;
}


//======================== TO SPLIT OR NOT TO SPLIT ===========================
