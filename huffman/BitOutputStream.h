/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 17, 2017
 * Overview: Header file for BitOutputStream.cpp
 * Assignment #3
 * File Name: BitOutputStream.h
 */

#ifndef BIT_OUTPUT_STREAM_H
#define BIT_OUTPUT_STREAM_H
#include <iostream>

/* 
 * Header file for the cpp file that can be used to write bits one at a time
 * to the output file
 * Code template taken from lecture 16 slides
 */
class BitOutputStream {

  private:
    char buf; // One byte buffer of bits
    int nbits; // How many bits have been written to buf
    std::ostream & out; // Reference to the output stream to use

  public:
    /* Initialize a BitOutputStream that will use the given ostream for output */
    BitOutputStream(std::ostream & os) : out(os), buf(0), nbits(0) {
      buf = 0; // Clear buffer
      nbits = 0; // Clear bit counter
    }

    /* Send the buffer to the output, and clear it */
    void flush();

    /* Write the least significant bit of the argument to the bit buffer, and 
     * increment the bit buffer index. But flush the buffer first, if it's full
     * Code based off of Stepik 7.4 reading assignment
     */
    void writeBit(unsigned int i);

};

#endif // BIT_OUTPUT_STREAM_H
