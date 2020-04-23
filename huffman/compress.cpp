/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 15, 2017
 * Overview: Contains methods to compress ASCII values with Huffman code
 * Assignment #3
 * File Name: compress.cpp
 */

#include <iostream>
#include <fstream>
#include "HCNode.h"
#include "HCTree.h"

using namespace std;

/* 
 * This program takes in 2 arguments: the input file and the output file. 
 * It encodes the string in the input file and writes a header and the encoded
 * message to the output file.
 */
int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Incorrect number of arguments. Expected 3, got " << argc << endl;
    return -1;
  }

  ifstream inFile;
  unsigned char nextChar;
  HCTree tree;
  std::vector<int> freqVec (256, 0); // Store 256 ints representing ASCII vals 
  inFile.open(argv[1], std::ios::binary);

  unsigned long long numBytes = 0;

  // Iterate through all characters in input file and increment frequency of 
  // each ASCII character accordingly
  while (1) {
    nextChar = (unsigned char)inFile.get();
    if (inFile.eof()) break;
    numBytes++;
    freqVec[(int)nextChar] += 1;
  }

  // Close the input file, clear error operators, and reset iterator
  inFile.close();
  inFile.clear();
  inFile.seekg(0, std::ios::beg);

  // Use frequencies of nonzero nodes to build Huffman tree
  tree.build(freqVec);
  ofstream outFile;
  outFile.open(argv[2], std::ios::binary);
  BitOutputStream outStream(outFile);

  // Write the frequency associated with each ASCII character into the output
  // file
  int numberOfLeaves = 0;
  for (int i = 0; i<freqVec.size(); i++){
    if(freqVec[i] > 0){
      numberOfLeaves++;
    }
  }

  outFile.put(numberOfLeaves);
  tree.encodeTree(outStream, outFile, numberOfLeaves);

  unsigned char mostSigByte = numBytes >> 56;
  unsigned char secondMostSigByte = numBytes >> 48;
  unsigned char thirdMostSigByte = numBytes >> 40;
  unsigned char fourthMostSigByte = numBytes >> 32;
  unsigned char fifthMostSigByte = numBytes >> 24;
  unsigned char sixthMostSigByte = numBytes >> 16;
  unsigned char seventhMostSigByte = numBytes >> 8;
  unsigned char leastSigByte = numBytes;

  outFile.put(mostSigByte);
  outFile.put(secondMostSigByte);
  outFile.put(thirdMostSigByte);
  outFile.put(fourthMostSigByte);
  outFile.put(fifthMostSigByte);
  outFile.put(sixthMostSigByte);
  outFile.put(seventhMostSigByte);
  outFile.put(leastSigByte);

  inFile.open(argv[1]);
  // Write the encoded message to the output file
  while (1) {
    nextChar = (unsigned char)inFile.get();
    //cout << "char: " << (int) nextChar << endl;
    if (inFile.eof()) break;
    //cout << "check" << endl;
    if(numBytes-- > 0){
      tree.encode(nextChar, outStream);
    }
  }
  outStream.flush();
  // Close ifstream and ofstream
  inFile.close();
  outFile.close();

  return 0;
}
