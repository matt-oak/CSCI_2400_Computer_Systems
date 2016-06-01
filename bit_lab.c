/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  // This is an application of DeMorgan's Law and can be shown via truth tables.
  return ~(~x & ~y);
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
  /* 0x55 corresponds to 01010101. We then do an OR statement on 0x55 (01010101) as well as 0x55 left-shifted 8 bits. This
   * left-shift corresponds to the value 0101010100000000. We then get the value 0101010101010101 and set 'word' equal to that value.
   * We then do another OR statement on 'word' and 'word' left-shifted 16 bits which would be the value 01010101010101010000000000000000.
   * When we return this OR statement we end up the value 0x55555555 which has all the even bits correspond with the number '1'.
   */ 
  int num = 0x55;
  int word = num | num << 8;
  return word | word << 16;
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  /* The hex number 0x00 corresponds to the binary value of 32 0's. When we use the ~ operator all of those 0's turn to 1's which
   * corresponds to the value 0xffffffff which in decimal is -1.
   */
  int a = 0x00;
  return ~a;
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  /* Initially I make a new integer corresponding to 0x55555555. I then use AND on x and 0x55555555 so all the even bits
   * of x get returned. I then do an XOR using 0x55555555 again so all 0's are returned. I then use ! on that final
   * value to convert the 0 to a 1.
   */
  int a1 = 0x55;
  int a2 = a1 | a1 << 8;
  int a3 = a2 | a2 << 16;
  return !((x & a3) ^ a3);
}
/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
  /* Much like allEvenBits, I make a new integer corresponding to 0xaaaaaaaa which has 1's in all the odd spots.
   * I then use AND on that hex number and the parameter in order to get all x's odd bits masked through. I then use BANG
   * on it to make it all 0's and BANG again to make the 0 become a 1 and then return it.
   */
  int a1 = 0xaa;
  int a2 = a1 | a1 << 8;
  int a3 = a2 | a2 << 16;
  return !(!(x & a3));
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  /*I decided it'd be easier to explain this one line by line*/
  
  int shifter = 0xff;							//Creating a 'string' of 8 1's
  int nShift = n << 3;							//New variable in order to shift the 8 1's over the proper amount, using << 3 because it is equivalent to multiplying by 8
  int mShift = m << 3;							//Same logic above
  int nByteLoc = (shifter << nShift) & x;		//Shifting the string of 8 1's over and using AND with the given integer to single out the proper bits from x
  int mByteLoc = (shifter << mShift) & x;		//Same logic above
  int combination = nByteLoc | mByteLoc;		//Combining these 16 bits into a sequence of 32 bits using OR
  int nByte = nByteLoc >> nShift;				//Right-shifting the byte corresponding to n back to the first 8 bit locations
  int mByte = mByteLoc >> mShift;				//Same logic as above
  int nByte2 = nByte & shifter;					//Using AND to get rid of any leading 0's
  int mByte2 = mByte & shifter;					//Same logic as above
  int nByte3 = nByte2 << mShift;				//Left-shifting the byte corresponding with n over to m's original position
  int mByte3 = mByte2 << nShift;				//Left-shifting the byte corresponding with m over to n's original position
  int combination2 = ~combination & x;			//Inverting combination and using AND with the original input to essentially 'get rid of' the bits in n's and m's locations
  return combination2 | mByte3 | nByte3;		//Combining everything together
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  /*Explained line by line*/
  
  int total = x + y;							//Getting the sum of x and y
  int xBit = x >> 31;							//Getting the sign bit for x
  int yBit = y >> 31;							//Getting the sign bit for y
  int totalShifted = total >> 31;				//Getting the sign bit for the sum of x and y
  int xyCompare = xBit ^ yBit;					//Checking to see if x and y have the same sign. An overflow will occur if they do.
  int xtotalCompare = xBit ^ totalShifted;		//Checking to see if x and the sum of x and y have the same sign. An overflow will occur if they have different signs.
  return !(~(xyCompare) & xtotalCompare);		//Using AND to check both of these conditions. Will only return 0 if x and y have the same sign and if x's sign and the sum's sign are different.
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /*Explained line by line*/
  
  int ones = (!!x << 31) >> 31;					//Creating a sequence of all 1's by double BANGing x to get 0x01, left-shifting 31 bits, and then arithmetically right-shifting back 31 bits to get all 1's
  int zeros = (!x << 31) >> 31;					//Creating a sequence of all 0's by BANGing x to get 0x00, left-shifting 31 bits, and then arithmetically right-shifting back 31 bits to get all 0's
  //int zeros = !x;
  //int ones = ~zeros;
  return ((ones & y) + (zeros & z));			//Mask y and then use AND on z with the sequence of 0's and then return their sum.
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*Explained line by line*/
  
  int lowerLimit = 0x2f;						//Creating a new variable corresponding to the hex value 1 below the lowest desired ASCII digit
  int y = x + ~lowerLimit;						//Adding x and invert the bits of lowerLimit. If there's an overflow (ie x is above 0x39) this will be negative 1
  int yBit = y >> 31;							//Getting the most significant bit to see if an overflow has occured
  int upperLimit = 0x3a;						//Creating a new variable corresponding to the hex value 1 above the highest desired ASCII digit
  int z = ~x + upperLimit;						//Inverting x and adding upperLimit, if overflow occurs this will be negative 1
  int zBit = z >> 31;							//Getting the most significant bit to see if an overflow has occured
  return !(yBit | zBit);						//Use OR between the leading y and z bit, BANG the answer, and return
}
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  /*Explained line by line*/
  
  int ones = 0xff;								//Creating a sequence of 8 1s
  int shift = n << 3;							//Creating a new variable in order to shift bits over the proper amount with respect to n
  int xBits = (ones << shift) & x;				//Shifting the sequence of 1s over the proper amount and masking x with it so just those bits of x pass through
  int invertedXBits = ~xBits;					//Inverting those bits
  int clearedX = x & invertedXBits;				//Again using AND in order to make byte 'n' all zeros in x
  int cShifted = (c << shift) | clearedX;		//Shift the desired bits over the proper amount and using OR in order to put those bits into the original x
  return cShifted;								//Return this value
}
/* reverseBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: reverseBits(0x11111111) = 0x88888888
 *            reverseBits(0xdeadbeef) = 0xf77db57b
 *            reverseBits(0x88888888) = 0x11111111
 *            reverseBits(0)  = 0
 *            reverseBits(-1) = -1
 *            reverseBits(0x9) = 0x90000000
 *  Legal ops: ! ~ & ^ | + << >> and unsigned int type
 *  Max ops: 90
 *  Rating: 4
 */
int reverseBits(int x) {
  int evenBits, oddBits, xEvenBits, xOddBits, shift1, shift2, deleter, nibblesSwapped, mask3, mask4, nibbleCombo1, nibbleCombo2, shift3, shift4, deleter2, bytesSwapped, mask5, mask6_1, mask6_2, mask6_3, mask6_4, mask6, byteCombo1, byteCombo2, shift5, shift6, deleter3, _2bytesSwapped, mask7_1, mask7_2, mask7, mask8_1, mask8_2, mask8, _2bytesCombo1, _2bytesCombo2, shift7, shift8, deleter4, _4bytesSwapped, mask9_1, mask9, mask10_1, mask10, _4bytesCombo1, _4bytesCombo2, shift9, shift10, deleter5, FINAL;
  evenBits = 0x55;
  evenBits = evenBits << 8 | evenBits;
  evenBits = evenBits << 16 | evenBits;
  oddBits = 0xaa;
  oddBits = oddBits << 8 | oddBits;
  oddBits = oddBits << 16 | oddBits;
  xEvenBits = evenBits & x;
  xOddBits = oddBits & x;
  shift1 = xEvenBits << 1;
  shift2 = xOddBits >> 1;
  deleter = 0xff;
  deleter = deleter << 8 | deleter;
  deleter = deleter << 15 | deleter;
  shift2 = shift2 & deleter;
  nibblesSwapped = shift1 | shift2;
  mask3 = 0x33;
  mask3 = mask3 << 8 | mask3;
  mask3 = mask3 << 16 | mask3;
  mask4 = 0xcc;
  mask4 = mask4 << 8 | mask4;
  mask4 = mask4 << 16 | mask4;
  nibbleCombo1 = nibblesSwapped & mask3;
  nibbleCombo2 = nibblesSwapped & mask4;
  shift3 = nibbleCombo1 << 2;
  shift4 = nibbleCombo2 >> 2;
  deleter2 = 0xff;
  deleter2 = deleter2 << 8 | deleter2;
  deleter2 = deleter2 << 14 | deleter2;
  shift4 = shift4 & deleter2;
  bytesSwapped = shift3 | shift4;
  mask5 = 0xf0;
  mask5 = mask5 << 8 | mask5;
  mask5 = mask5 << 16 | mask5;
  mask6_1 = 0xf;
  mask6_1 = mask6_1 << 24;
  mask6_2 = 0xf;
  mask6_2 = mask6_2 << 16;
  mask6_3 = 0xf;
  mask6_3 = mask6_3 << 8;
  mask6_4 = 0xf;
  mask6 = mask6_1 | mask6_2 | mask6_3 | mask6_4;
  byteCombo1 = bytesSwapped & mask5;
  byteCombo2 = bytesSwapped & mask6;
  shift5 = byteCombo1 >> 4;
  shift6 = byteCombo2 << 4;
  deleter3 = 0xff;
  deleter3 = deleter3 << 8 | deleter3;
  deleter3 = deleter3 << 12 | deleter3;
  shift5 = shift5 & deleter3;
  _2bytesSwapped = shift5 | shift6;
  mask7_1 = 0xff;
  mask7_1 = mask7_1 << 24;
  mask7_2 = 0xff;
  mask7_2 = mask7_2 << 8;
  mask7 = mask7_1 | mask7_2;
  mask8_1 = 0xff;
  mask8_1 = mask8_1 << 16;
  mask8_2 = 0xff;
  mask8 = mask8_1 | mask8_2;
  _2bytesCombo1 = _2bytesSwapped & mask7;
  _2bytesCombo2 = _2bytesSwapped & mask8;
  shift7 = _2bytesCombo1 >> 8;
  shift8 = _2bytesCombo2 << 8;
  deleter4 = 0xff;
  deleter4 = deleter4 << 8 | deleter4;
  deleter4 = deleter4 << 8 | deleter4;
  shift7 = shift7 & deleter4;
  _4bytesSwapped = shift7 | shift8;
  mask9_1 = 0xff;
  mask9_1 = mask9_1 << 8 | mask9_1;
  mask9 = mask9_1 << 16;
  mask10_1 = 0xff;
  mask10 = mask10_1 << 8 | mask10_1;
  _4bytesCombo1 = _4bytesSwapped & mask9;
  _4bytesCombo2 = _4bytesSwapped & mask10;
  shift9 = _4bytesCombo1 >> 16;
  shift10 = _4bytesCombo2 << 16;
  deleter5 = 0xff;
  deleter5 = deleter5 << 8 | deleter5;
  shift9 = shift9 & deleter5;
  FINAL = shift9 | shift10;
  return FINAL;
}
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
  int sum = x + y;
  int xBit = (x >> 31) & 1;
  int yBit = (y >> 31) & 1;
  int sumBit = (sum >> 31) & 1;
  int xyCompare = xBit ^ yBit;						//true if no overflow
  int xsumCompare = xBit ^ sumBit;					//
  int no_overflow = xyCompare & ~(xsumCompare);
  return !(no_overflow);
  
}
/*
 * Extra credit
 */
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
  return 2;
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
  return 2;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  return 2;
}
