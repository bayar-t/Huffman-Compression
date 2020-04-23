/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 15, 2017
 * Overview: Contains methods to implement a HCTree
 * Assignment #3
 * File Name: HCTree.cpp
 */

#include "HCTree.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {

  // Create a priority queue
  std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> pq;

  // Push all symbols with nonzero frequencies as nodes into priority queue
  for (int i = 0;  i < freqs.size(); i++) {
    if (freqs[i] > 0) {
      HCNode* n = new HCNode(freqs[i], (unsigned char)i);
      leaves[i] = n;
      pq.push(n);
    }
  }

  // Remove the two lowest count nodes, sum their counts into a new node,
  // and insert this node into the priority queue
  while (pq.size() > 1) {
    HCNode* node1 = pq.top();
    pq.pop();
    HCNode* node2 = pq.top();
    pq.pop();
    int sum = node1->count + node2->count;
    HCNode* parent = new HCNode(sum, node1->symbol, node1, node2, NULL);

    node1->p = parent;
    node2->p = parent;
    pq.push(parent);
  }

  // Root points to root of the trie
  if (pq.empty()) {
    root = NULL;
  }
  else {
    root = pq.top();
  }
}

void HCTree::buildFromRoot(HCNode* currNode){
  root = currNode;
}




/* Use recursion to post-order traverse the tree 
 * PRECONDITION: build() has been called, to create the coding tree, and
 * intialize root pointer and leaves vector. Used as a helper function
 * inside encodeTree function.
 */
void HCTree::encodeTreeHelper(HCNode* currNode, BitOutputStream& out, 
                      ofstream& ofOut, int numChars) {
  
  // Base case: currNode is leaf node
  // Write the bit '0' and the ASCII value of current node's symbol as an 8-bit
  // binary encoding to the output file
  if (!currNode->c0 && !currNode->c1) {
    out.writeBit(0);

    unsigned char currSymbol = currNode->symbol;
    for (int i=0; i<8; i++) {
      out.writeBit(currSymbol >> (7-i));
    }

    numLeavesEncoded++;

    // Encoding the last leaf in the tree structure, so flush
    if (numLeavesEncoded == numChars) {
      out.flush();
    }

    return;
  }

  if (currNode->c0 && currNode->c1) {
    out.writeBit(1);
  }
  encodeTreeHelper(currNode->c0, out, ofOut, numChars);
  encodeTreeHelper(currNode->c1, out, ofOut, numChars);
}

/** Encode the structure of the tree, and write the encoding to the output file
 *  PRECONDITION: build() has been called, to create the coding tree, and
 *  initialize root pointer and leaves vector.
 */
void HCTree::encodeTree(BitOutputStream& out, ofstream& ofOut, int numChars) {

  if (root == NULL || leaves.empty()) {
    return;
  }

  HCNode* currNode = this->root;
  encodeTreeHelper(currNode, out, ofOut, numChars);

}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {

  // Edge case: if build() was never called, root would buf(be null and the
  // leaves vector would be empty
  if (root == NULL || leaves.empty()) {
    return;
  }

  // Edge case: only one node in tree
  if (root->c0 == NULL && root->c1 == NULL) {
    out.writeBit(0);
    out.flush();
    return;
  }

  // temp points to the vector containing the parameter symbol
  HCNode* temp = NULL;
  string encoded;
  // Find the symbol in the leaves vector, and make temp point to the node
  // with that symbol
  temp = leaves[(int)symbol];

  if (!temp) {
    std::cout << "Symbol was not found in leaves vector\n";
    return;
  }

  // Starting from leaf node, move up the tree until root node is reached
  // Each time node is moved up:
  // Append '0' if node is left child of its parent
  // Append '1' if node is right child of its parent 
  while (temp->p != NULL) {
    encoded += (temp->p->c0 == temp) ? '0' : '1';
    temp = temp->p;
  }

  // Output the reverse of the encoded string
  for (int i = encoded.size()-1; i >= 0; i--){
    const char encodedChar = encoded.at(i);

    out.writeBit((int)encodedChar-'0');
  }
  //out.flush();

}

/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
 *  BE USED IN THE FINAL SUBMISSION.
 */
void HCTree::encode(byte symbol, ofstream& out) const {

  // Edge case: if build() was never called, root would buf(be null and the
  // leaves vector would be empty
  if (root == NULL || leaves.empty()) {
    return;
  }

  // Edge case: only one node in tree
  if (root->c0 == NULL && root->c1 == NULL) {
    out.put('0');
    return;
  }

  // temp points to the vector containing the parameter symbol
  HCNode* temp = NULL;
  string encoded = "";
  // Find the symbol in the leaves vector, and make temp point to the node
  // with that symbol
  temp = leaves[(int)symbol];
  if (!temp) {
    std::cout << "Symbol was not found in leaves vector\n";
    return;
  }

  // Starting from leaf node, move up the tree until root node is reached
  // Each time node is moved up:
  // Append '0' if node is left child of its parent
  // Append '1' if node is right child of its parent 
  while (temp->p != NULL) {
    encoded += (temp->p->c0 == temp) ? '0' : '1';
    temp = temp->p;
  }
  // Output the reverse of the encoded string
  for (int i = encoded.size()-1; i >= 0; i--){
    out.put(encoded.at(i));
  }

}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const {
  // Edge case: if build() was never called, root would be null
  if (root == NULL) { //|| leaves.empty()){
    return -1;
  }

  // Edge case: only one node in tree
  if (root->c0 == NULL && root->c1 == NULL) {
    int tempInt = in.readBit();
    return (int)root->symbol;
  }
  HCNode* temp = root;
  bool endWhileLoop = false;

  // Read input file one byte at a time
  // Also exit loop if end of tree is reached
  while (temp->c0 && temp->c1 ) {
    // Read text-based number from ifstream as a number
    int intEquivalent = in.readBit();
    if (intEquivalent == 0) {
      temp = temp->c0;
    }
    else if (intEquivalent == 1) {
      temp = temp->c1;
    }
    else {
      return -1;
    }

  }

  return (int)temp->symbol;
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the ifstream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
 *  IN THE FINAL SUBMISSION.
 *  If root is null or leaves vector has no nodes, return -1
 */
int HCTree::decode(ifstream& in) const {
  // Edge case: if build() was never called, root would be null and the
  // leaves vector would be empty
  if (root == NULL || leaves.empty()){
    return -1;
  }

  // Edge case: only one node in tree
  if (root->c0 == NULL && root->c1 == NULL) {
    char tempChar = in.get();
    if (in.eof()) {
      return -1;
    }
    else {
      return (int)root->symbol;
    }
  }
  HCNode* temp = root;
  bool endWhileLoop = false;
  // Read input file one byte at a time
  // Also exit loop if end of tree is reached
  while (!endWhileLoop) {
    // Read text-based number from ifstream as a number
    char charEquivalent = in.get();
    if (charEquivalent == '0') {
      temp = temp->c0;
    }
    else if (charEquivalent == '1') {
      temp = temp->c1;
    }
    else {
      return -1;
    }

    //  If temp points to a leaf, exit while loop because we've reached the
    //  end of the tree
    if (!temp->c0 && !temp->c1){
      endWhileLoop = true;
      break;
    }
    if (endWhileLoop) {
      break;
    }

  }
  return (int)temp->symbol;
}

/* Helper method for destructor. Takes a pointer to a node and deletes it 
 * if it is not null, and recursively deletes all of the current node's 
 * children */
void HCTree::deleteAll(HCNode* aNode) {
  if (aNode == NULL){
    return;
  }
  if (aNode->c0){
    deleteAll(aNode->c0);
  }
  if (aNode->c1){
    deleteAll(aNode->c1);
  }
  delete(aNode);
}

/* Destructor */
HCTree::~HCTree() {
  if (root) {
    deleteAll(root);
  }
}

