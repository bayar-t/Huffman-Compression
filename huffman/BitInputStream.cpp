/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 17, 2017
 * Overview: Reads bits one at a time from the input file
 * Assignment #3
 * File Name: BitInputStream.cpp
 */

#include "BitInputStream.h"

  /* Fill the buffer from the input */
  void BitInputStream::fill() {
    buf = in.get();
    nbits = 0;
  }

  /* Read the next bit from the bit buffer
   * Fill the buffer from the input stream first if needed
   * Return 1 if the bit read is 1
   * Return 0 if the bit read is 0
   * Code based off of Stepik 7.4 Reading Assignment
   */
  int BitInputStream::readBit() {
    // If all bits in the buffer are read, fill the buffer first
    if (nbits == 8) {
      fill();
    }

    // Get the next unread bit from the bitwise buffer
    // That is, get the bit at the appropriate location in the bit buffer
    unsigned int nextBit = buf >> (7-nbits);
    nextBit = nextBit & 1;

    // Increment the index
    nbits++;

    // Return the bit we just read
    return nextBit;
  }

