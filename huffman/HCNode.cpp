/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 15, 2017
 * Overview: Contains methods to create a node for the HCTree
 * Assignment #3
 * File Name: HCNode.cpp
 */

#include "HCNode.h"

/** Less-than comparison, so HCNodes will work in std::priority_queue
 *  We want small counts to have high priority.
 *  And we want to break ties deterministically.
 */
bool HCNode::operator<(const HCNode& other) {
  // Code taken from lecture 13, page 21
  // If counts are different, just compare counts
  if (count != other.count) return count > other.count;

  // Counts are equal. Use symbol value to break tie.
  return symbol < other.symbol;
}
