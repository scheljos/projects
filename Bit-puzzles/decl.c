#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = {
 {"bitMatch", (funct_t) bitMatch, (funct_t) test_bitMatch, 2, "& ~ |", 14, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"magicNumber", (funct_t) magicNumber, (funct_t) test_magicNumber, 0,
    "! ~ & ^ | + << >>", 3, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"copyLSByte", (funct_t) copyLSByte, (funct_t) test_copyLSByte, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"isNonNegative", (funct_t) isNonNegative, (funct_t) test_isNonNegative, 1,
    "! ~ & ^ | + << >>", 6, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"isLowercase", (funct_t) isLowercase, (funct_t) test_isLowercase, 1,
    "! ~ & ^ | + << >>", 15, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"isUppercaseConsonant", (funct_t) isUppercaseConsonant, (funct_t) test_isUppercaseConsonant, 1,
    "! ~ & ^ | + << >>", 15, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"satUMul3", (funct_t) satUMul3, (funct_t) test_satUMul3, 1,
    "~ & ^ | + << >>", 20, 3,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
 {"floatUChar2Float", (funct_t) floatUChar2Float,
     (funct_t) test_floatUChar2Float, 1,
    "$", 25, 4,
     {{0, 255},{1,1},{1,1}}},
  {"", NULL, NULL, 0, "", 0, 0,
   {{0, 0},{0,0},{0,0}}}
};
