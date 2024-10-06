/* 
 * INF559 Data Lab 
 * 
 * <Please put your name and userid here>  name: Garance Perrot , userid: garance.perrot
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

/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
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
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* bit manipulation */
/*
 * func1 - returns 1 if x == 0, and 0 otherwise 
 *   Examples: func1(5) = 0, func1(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int func1(int x) {
  //we use the built-in operator ! 
  return !x;
}
/* 
 * func2 - ~(x|y) using only ~ and & 
 *   Example: func2(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int func2(int x, int y) {
  //bit operations work similarly to propositions, not (a or b) is equivalent to (not a) and (not b)
  return (~x) & (~y) ;
}
/* 
 * func3 - x&~y using only ^ and &
 *   Example: func3(1,2) = 1, func3(0,1) = 0, 0 
 *   Legal ops: & ^
 *   Max ops: 8
 *   Rating: 1
 */
int func3(int x, int y) {
  // recall:  x&y=1 iff x=1 and y=1,
  //          x^y=1 iff x and y are different
  //I used truth tables trying different combinations of using &,^ to find to relation below. 
  return (y ^ ( x^y)) ^ (x&y);
}
/* 
 * func4 - swap the first 16 bits of x with the last 16 bits of x
 *   Examples: func4(0x87654321) = 0x43218765, func4(0x12345678) = 0x56781234
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 2
 */
int func4(int x) {
  //we perform a right shift on the left part of the bit , and a left shift on the right part of the bit
  //but the right shift is arithmetic so we have to get rid of the additional ones that were added
  //we use a mask for this : 0xFF is composed of eigth ones in binary
  // so mask =((0xFF)<<8) + 0xFF is in binary 0...01...1  with 16 zeros on the left and 16 ones on the right
  //We "&" it to the arithmetic right shift to fix the problem of ones on the left side ( 1&0 = 0)
  //then we put together the two parts (right and let shift of x) with an "or" operator that "concatenates" two bits
  int mask = ((0xFF)<<8) + 0xFF;  
  int right_shift = (x >> 16) & mask;
  int res =  right_shift | (x << 16);
  return res;
}
/* 
 * func5 - set all bits of result to least significant bit of x
 *   Example: func5(5) = 0xFFFFFFFF, func5(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int func5(int x) {
  //we first extract the least significant bit of x by "&"ing it with 1. 
  // if lsb = 0...0 , then ~lsb = 1..1 (in binary) and adding 1 to the bit returns 0..0 
  //else if lsb = 0...01 ; then ~lsb = 1...10 and adding 1 to the bit returns 1...1
   int lsb = x & 1;
   int res = ~lsb +1;
   return res;
}
/* 
 * func6 - return 1 if all even-numbered bits in word set to 1
 *   Examples func6(0xFFFFFFFE) = 0, func6(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int func6(int x) { 
  //In hexadecimal notation, we notice that 0x55 is the bit of length 8 composed of zeros for odd bits and ones for even bits
  //we use shift operators and additions to create a mask which has the same form as 0x55 but of length 32. 
  //if all even positions of x are one: -then x&mask returns the mask (for every value in odd positions)
  //                                    - xoring it with the mask returns 0 (as y^y=0 for all y)
  //                                    - using !y operator returns 1 if y is 0
  //otherwise: - x&mask returns the mask but with "1" bits that are missing
  //           - xoring it with the mask returns a non-zero bit
  //           - using !y operator returns 0 for all y!=0 
    int m = ((0x55)<<8) + 0x55;
    int mask = (m << 16) + m;  
    return !((x & mask)^mask);
}
/* 
 * func7 - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: func7(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */

int func7(int x, int n) {  
  //we first perform the arithmetic right shift by n. But we have to erase the additionial ones on the left of y. 
  // For this we create a mask with n zeros on the left and ones for the other bits.
  //Finally "&"ing the mask with y erases the additionial ones in y. 
  int y = x>>n;
  int m = ((1 << 31) >> n) << 1;
  int mask = ~m;
  return y & mask;
}


/* 2's complement */
/* 
 * func8 - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int func8(void) {
//we search for the negative value of the bit composed of 32 ones. In 2s complement, the negative value is (logic complement +1)mod 2^wordsize
// and the most significant bit specifies the sign (1 for negative, 0 for positive)
// so the bit with 10...0 is the minimum 2s complement integer, that we obtain by a left shift of 1 over the other 31 bits. 
  return 1 << 31;
}
/*
 * func9 - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int func9(int x) {
  //first try : return !(~(1<<31) ^ x) but illegal operator
  // we deduce from the previous question that the maximum in 2 s complement is ~(1<<31). 
  // if x is the maximum , xoring it with itself gives 0 and using ! opertor gives 1 
  // otherwise the xor returns a non-zero bit and ! returns 0

  //second try: 
  // the maximum in 2s complement is 011...11 with 31 ones on the right. 
  // if x=0x7FFFFFFF then x+1^x is only the 32 bit only composed of ones and "a" is the zero bit.
  //                 then "b" is zero as x+1 is non zero
  //                 and a+b is zero so "c" returns 1
  //otherwise: a is not the zero bit so it follows that c returns 0
  int a = ~((x + 1) ^ x); 
  int b = !(x + 1);
  int c = !(a + b);

  return c; 
}
/* 
 * func10 - return -x 
 *   Example: func10(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int func10(int x) { 
   //By definition in the 2s complement representation , the negative value of x is (logic complement of x +1)mod 2^wordsize
  return (~x) + 1;
}
/* 
 * func11 - Determine if can compute x+y without overflow
 *   Example: func11(0x80000000,0x80000000) = 0,
 *            func11(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int func11(int x, int y) {
  // we extract the most significant bit out of the bits x , y  and their sum
  //here we do not care about the additional ones from arithmetic right shift beacuse only the last bit matters in the following comparisons:
  //   - if msb_x and msb_y are both 0 and msb_sum is 1 , there will be an overflow
  //   - if msb_x and msb_y are both 1 and msb_sum is 0 , there will be an overflow
  //in the two cases above, we should return 0  : 
  //One of the two "or" comparisons give 0 , the other one gives 1, "&"ing both gives 0 and !! returns 0 as wanted. 
  //   - Otherwise,  both comparisons give 1, "&"ing them gives 1 
  //(but actually returns s-1 in some cases  in 2s complement, so we have to fix the problem by adding !! as !(-1) returns 1 and !1 returns 0. )

  int msb_sum = (x + y) >> 31;
  int msb_x  = x >> 31;
  int msb_y = y >> 31;
  return !!((~msb_sum | msb_x | msb_y) & (msb_sum | ~msb_x | ~msb_y));
}
/* 
 * func12 - if x > y  then return 1, else return 0 
 *   Example: func12(4,5) = 0, func12(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int func12(int x, int y) {
  
//We create a variabe temp which returns 1 if x > y but if we do not take into account the sign.
//Then we extract from x and y their most significant bit (sign bit).
// - if x>0 (msb_x = 0) and y<0 (msb_y = 1) then we return 1 no matter the value of temp
// - x < 0 (msb_x = 1) and y >0 (msb_y = 0) then we should return 0 no matter the value of temp
// otherwise we return the value of temp
  int temp = !((x + ~y) >> 31);
  int msb_x = x >> 31;
  int msb_y = y >> 31;
  return ((!msb_x) & msb_y) | (temp & ((!msb_x) | msb_y));
}
/*
 * func13 - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: func13(0x40000000,0x40000000) = 0x7fffffff
 *             func13(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int func13(int x, int y) {
  //We start by summing x and y
  //Then we check for the sign of the overflow  : we take the most significant bits (sign bits ) of sum^x and of sum^y 
  //if the overflow is positive then the left shift by 31 returns the maximum positive value
  //if the overflow is negative and we should return the minimum possible value
	int sum = x+y;
	int check=((sum^x)>>31)&((sum^y)>>31);
	return (sum>>(check & 31)) + (check <<31);
}
