/*
 * CS:APP Data Project
 *
 * Joe Schelske schel351 5463525
 *
 * bits.c - Source file with your solutions to the project.
 *          This is the file you will hand in on Canvas.
 *
 * WARNING: Do not include <stdio.h> or other headers; they will
 * confuse the dlc compiler. You can still use printf for debugging
 * without including <stdio.h>, because we've declared it below.
 */

int printf(const char *fmt, ...);

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Project by
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
  one operator per line. Notice however that all the lines that declare
  variables (the ones starting with "int") need to come before any lines that
  do not declare variables. Modern C and C++ compilers are more permissive
  in allowing declarations and statements to be intermixed, but you need to
  follow the strict separation of putting all the declarations first for
  compatibility with the DLC that checks the coding rules.

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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use btest to verify that your solutions produce
 *      the correct answers.
 */


#endif
/*
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only basic bitwise operations ~, |, and &.
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
  /*
  x & y gets all the 1 bits x and y have in common. Or this with the 0 bits the two
  have in common to get the bits that match.
  */
  return (x & y) | (~(x | y));
}
/*
 * magicNumber - return the bit pattern 0xfff2ffff
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 3
 *   Rating: 1
 */
int magicNumber(void) {
  /*
  13 is 1101 in binary, and ~13 is 0010 (or 2 in decimal). Shift 13 left by 16
  and negate it to get all 1s with 0010 (2) in the right position.
  */
  int x = 13;
  return ~(x << 16);
}
/*
 * copyLSByte - set all bytes of result to least significant byte of x
 *   Example: copyLSByte(5) = 0x05050505, copyLSByte(-2) = 0xfefefefe
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSByte(int x) {
  /*
  0xFF is & with x to get only the LSByte in x.
  Save this value in another variable, shift the original by 8, add the saved value.
  Save this value, now 0x0000XYXY. Shift the original by 16, now 0xXYXY0000. Add the
  saved value, get 0xXYXYXYXY. Return.
  */
  int helper = 255;
  int lsbyte = x & helper;
  int save_lsbyte = lsbyte;
  lsbyte <<= 8;
  lsbyte += save_lsbyte;
  save_lsbyte = lsbyte;
  lsbyte <<= 16;
  lsbyte += save_lsbyte;
  return lsbyte;
}
/*
 * isNonNegative - return 1 if x >= 0, return 0 otherwise
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNonNegative(int x) {
  /*
  Right shift by 31 to get bitstring of only MSB of x repeated. If x was positive, this will be
  0, and if x was negative, this will be -1. Adding 1 to this value gives the desired output
  immediately.
  */
  return (x >> 31) + 1;
}
/*
 * isLowercase - return 1 if x as ASCII code is a lowercase letter.
 *   Example: isLowercase(0x35) = 0.
 *            isLowercase(0x61) = 1.
 *            isLowercase(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isLowercase(int x) {
  /*
  The u and v lines compare x to 97 and 123. A value is a lowercase letter if it is between
  97 and 122, inclusive. Right shifting by 31 accesses only the MSB, 1 for negatives and 0
  for positives. There is no theory behind the logic in the y line; it is simply what I came
  up with so that every case for x gives me the intended result. The cases were:

  x is between 97 and 122 inclusive, return 1
  x is less than 97, return 0
  x is greater than 122, return 0
  x is so small that subtracting the bounds causes overflow
  */
  int u = x + (~97 + 1);
  int v = x + (~123 + 1);
  int y = ((u >> 31) + 1) & (v >> 31);
  return y;
}

/*
 * isUppercaseConsonant - return 1 if x as an ASCII code is an uppercase
 *     letter, but not a vowel A, E, I, O, or U. As an exception to the usual
 *     rules for integer problems, you may use any integer constants, even
 *     big ones.
 *   Example: isUppercaseConsonant(0x35) = 0. ('5')
 *            isUppercaseConsonant(0x4D) = 1. ('M')
 *            isUppercaseConsonant(0x55) = 0. ('U')
 *            isUppercaseConsonant(0x59) = 1. ('Y')
 *   Legal ops: ! ~ & ^ | + << >>, also large constants
 *   Max ops: 15
 *   Rating: 3
 */
int isUppercaseConsonant(int x) {
  /*
  I used too many operations to do this one because I couldn't figure out a way
  to do it in less. It is exactly the same as isLowercase, except the bounds I check between
  are 65 and 91 (the ASCII range of uppercase), and there's an extra line that checks
  if the input int x is the same as a vowel. The logic here is that a number xor with itself
  will give 0, and !0 gives 1. Since I would at this point return a 1, I want to subtract 1 to
  return 0 as vowels do not count (accomplished using the ~x + 1 trick).
  */
  int u = x + (~65 + 1);
  int v = x + (~91 + 1);
  int y = (((u >> 31) + 1) & (v >> 31));
  int z = ~(!(65 ^ x) + !(69 ^ x) + !(73 ^ x) + !(79 ^ x) + !(85 ^ x)) + 1;
  int ret = y + z;
  return ret;
}


/*
 * satUMul3 - multiplies by 3, saturating to UMax if unsigned overflow
 *  Examples: satUMul3(0x10000000) = 0x30000000
 *            satUMul3(0x50000000) = 0xf0000000
 *            satUMul3(0x60000000) = 0xFFFFFFFF (Saturate to UMax)
 *            satUMul3(0xFFFFFFFF) = 0xFFFFFFFF (Saturate to UMax)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satUMul3(int x) {
  /*
  First, I create UMax / 3 using a process similar to the one used in copyLSByte.
  If x is a positive number greater than this, or a negative number at all (remembering
  that a negative number in signed integer representation would be greater than UMax / 2
  in unsigned representation), then (checkOverflow1 | checkOverflow2) will be 0xffffffff,
  meaning that result | (0xffffffff) will be 0xffffffff because any number or a string of
  all 1s will be a string of all 1s.
  If both of these "tests" (checkOverflow1 and checkOverflow2) pass, meaning they both are 0,
  x + x + x is returned.
  */
  int checkOverflow1;
  int checkOverflow2;
  int result;
  int create_umax_div3 = 0x55;
  int save_lsbyte = create_umax_div3;
  create_umax_div3 <<= 8;
  create_umax_div3 += save_lsbyte;
  save_lsbyte = create_umax_div3;
  create_umax_div3 <<= 16;
  create_umax_div3 += save_lsbyte;
  checkOverflow1 = x >> 31;
  checkOverflow2 = create_umax_div3 + ~x + 1;
  checkOverflow2 >>= 31;
  result = x + x + x;
  return result | (checkOverflow1 | checkOverflow2);
}
/*
 * floatUChar2Float - Return bit-level equivalent of expression (float) x,
 *   where x is an unsigned char between 0 and 255.
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 4
 */
unsigned floatUChar2Float(unsigned char b) {
  /*
    (-1)^s * M * 2^E

    The MSB (sign bit) will always be 0 because all unsigned chars are positive.
    The exponent field is encoding by calculating E + Bias (which is 127).
    I find E by dividing the input unsigned char b by 2 iteratively until b < 2.
    After the while loop, I add the E I found to 127 and shift it over by 23 so that
    it is at the position appropriate for a single-precision float. The tricky part was
    encoding the frac field. I do this by doing b % 2 in the while loop; if 2 does not
    divide b evenly at any point, I know the frac field will have a non-zero digit. I
    used the floating point calculator h-schmidt.net/FloatConverter/IEEE754.html to
    observe the position where a 1-bit is added to the frac field every time a modulus
    operation on b gives 1. To make sure that the most significant fractional bits
    are where they are supposed to be, I shift the frac_field variable over by 23
    minus the value in frac_shift.

    I handle the one and only denormalized value 0 by setting the field_exp to 0 if b
    is 0.
  */
  unsigned int field_exp;
  unsigned int field_e = 0;
  unsigned int frac_shift = 0;
  unsigned int frac_field = 0;
  unsigned int mod2;
  if (b == 0) {
    field_exp = 0;
  } else {
    field_exp = 127;
  }
  while (b >= 2) {
    mod2 = b % 2;
    frac_field += (mod2 << frac_shift);
    frac_shift++;
    b /= 2;
    field_e++;
  }
  frac_field <<= 23 - frac_shift;
  field_exp += field_e;
  field_exp <<= 23;
  return field_exp + frac_field;
}

/* Inside this comment, cut and paste the final report obtained from
   running "perl check.pl" showing how your assigned was auto-graded:

   Correctness Results	Perf Results
Points	Rating	Errors	Points	Ops	Puzzle
1	1	0	2	4	bitMatch
1	1	0	2	2	magicNumber
2	2	0	2	5	copyLSByte
2	2	0	2	2	isNonNegative
3	3	0	2	10	isLowercase
3	3	0	0	27	isUppercaseConsonant
3	3	0	2	13	satUMul3
4	4	0	2	13	floatUChar2Float

Raw Score = 33/35 [19/19 Corr + 14/16 Perf] (76 total operators)

Username: schel351
Host: csel-vole-41.cselabs.umn.edu
Time: Mon May  4 18:06:08 2020
bits.c checksum: ac50dca21f449610a4d6b67b9f45213a5c029ca2

Scaled auto-grading score = 66/70

Report checksum: 8cf57cc99687c2c01d6bca88a3dcac37026c5ab2


*/
