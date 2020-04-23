/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 17, 2017
 * Overview: Writes bits one at a time to the output file 
 * Assignment #3
 * File Name: BitOutputStream.cpp
 */

#include "BitOutputStream.h"

  /* Send the buffer to the output, and clear it */
  void BitOutputStream::flush() {
    out.put(buf);
    out.flush();
    buf = nbits = 0;
  }

  /* Write the least significant bit of the argument to the bit buffer, and 
   * increment the bit buffer index. But flush the buffer first, if it is full
   * The parameter, i, is always either a 0 or 1
   *
   * Code based off of Stepik 7.4 reading assignment
   */
  void BitOutputStream::writeBit(unsigned int i) {
    // Is the bit buffer full? Then flush it
    if (nbits == 8) {
      flush();
    }

    // Write the least significant bit of i into the buffer at the current 
    // index, from left to right
    buf = buf | ((i&1) << (7-nbits));
    
    // Increment the index (i.e. # of bits in bitwise buffer)
    nbits++;
  }

