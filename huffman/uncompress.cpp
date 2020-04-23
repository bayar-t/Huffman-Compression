/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 15, 2017
 * Overview: Contains methods to uncompress Huffman code to ASCII values
 * Assignment #3
 * File Name: uncompress.cpp
 */

#include <iostream>
#include <fstream>
#include "HCNode.h"
#include "HCTree.h"

int numLeaves = 0;
/* 
 * Given the root node, an ifstream and BitInputStream, build the Huffman tree
 * using the encoded tree structure
 */
void buildTree(HCNode* currNode, ifstream& in, BitInputStream& bitIn, 
               int bitPosition){

  if (bitIn.readBit() == 0) {
    string next8Bits = "";
    // Read the next 8 bits
    for (int i=0; i<8; i++) {
      next8Bits += std::to_string(bitIn.readBit());
    }


    char aByte = 0; // next 8 bits expressed as a byte
    const char* charPtr = next8Bits.c_str(); // convert string to a const char*

    for (int i=0; i<8; i++) {
      aByte = aByte | (((int)(*(charPtr+i))-'0') << (7-i));
    }

    currNode->symbol = aByte;
    if(--numLeaves <= 0){
      return;
      }
  }

  else {
    currNode->c0 = new HCNode(0, 0);
    currNode->c1 = new HCNode(0, 0);

    buildTree(currNode->c0, in, bitIn, bitPosition);
    buildTree(currNode->c1, in, bitIn, bitPosition);
  }

}

/* 
 * This program takes in 2 arguments: the input file and the output file.
 * It decodes the encoded message from the input file and writes the decoded
 * message to the output file.
 */
int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Incorrect number of arguments. Expected 3, got " << argc << endl;
    return -1;
  }

  ifstream inFile;
  std::vector<int> freqVec (256, (int)0); // Store 256 ints
  inFile.open(argv[1], std::ios::binary);

  // Go through first 256 lines, and put the associated frequencies into vector
  int theFreq = 0;
  int freqCheck = 0;
  HCNode* root = new HCNode(0, 0);
  BitInputStream inStream(inFile);
  
  numLeaves = (int)inFile.get();
  HCTree tree;
  buildTree(root, inFile, inStream, numLeaves);
  tree.buildFromRoot(root);

  unsigned long long numCharsToDecode = 0;
  unsigned long long mostSigByte = inFile.get();
  mostSigByte = mostSigByte << 56;
  unsigned long long secondMostSigByte = inFile.get();
  secondMostSigByte = secondMostSigByte << 48;
  unsigned long long thirdMostSigByte = inFile.get();
  thirdMostSigByte = thirdMostSigByte << 40;
  unsigned long long fourthMostSigByte = inFile.get();
  fourthMostSigByte = fourthMostSigByte << 32;
  unsigned long long fifthMostSigByte = inFile.get(); 
  fifthMostSigByte = fifthMostSigByte << 24;
  unsigned long long sixthMostSigByte = inFile.get();
  sixthMostSigByte = sixthMostSigByte << 16;
  unsigned long long seventhMostSigByte = inFile.get();
  seventhMostSigByte = seventhMostSigByte << 8;
  unsigned long long leastSigByte = inFile.get();

  numCharsToDecode = mostSigByte | secondMostSigByte | thirdMostSigByte |
                     fourthMostSigByte | fifthMostSigByte | sixthMostSigByte |
                     seventhMostSigByte | leastSigByte;

  BitInputStream inStream2(inFile);

  int decodedVal = 0;
  ofstream outFile;
  outFile.open(argv[2]);
  string decodedString;
  // Write the decoded message to the output file
  unsigned long long numDecoded = 0;
  while (1) {
    if(inFile.eof()) break;
    decodedVal = tree.decode(inStream2);
    // Edge case: don't write anything that's not an ASCII character to the
    // output file
    if (decodedVal != -1) {
      // Every time a character is inserted, increment check
      // Make sure to not write more decoded bits than it should
        if (numDecoded < numCharsToDecode) {
          outFile << (char)decodedVal;
          numDecoded++;
        }
    }
    else {
      break;
    }
  }

  // Close ifstream and ofstream
  inFile.close();
  outFile.close();

  return 0;
}
