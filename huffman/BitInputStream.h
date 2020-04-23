/*
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 17, 2017
 * Overview: Header file for BitInputStream.cpp
 * Assignment #3
 * File Name: BitInputStream.h
 */

#ifndef BIT_INPUT_STREAM_H
#define BIT_INPUT_STREAM_H
#include <iostream>

/* Header file for the cpp file that can be used to read bites one at at time
 * Code template taken from lecture 15 slides
 */
class BitInputStream {

  private:
    char buf; // One byte buffer of bits
    int nbits; // How many bits have been written to buf
    std::istream & in; // Reference to the input stream to use

  public:
    /* Initialize a BitInputStream that will use the given istream for input */
    BitInputStream(std::istream & is) : in(is), buf(0), nbits(8) {}

    /* Fill the buffer from the input */
    void fill();

    /* Read the next bit from the bit buffer
     * Fill the buffer from the input stream first if needed
     * Return 1 if the bit read is 1
     * Return 0 if the bit read is 0
     * Code based off of Stepik 7.4 Reading Assignment
     */
    int readBit();

};

#endif // BIT_INPUT_STREAM_H
