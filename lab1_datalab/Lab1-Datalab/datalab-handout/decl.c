#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = {

 {"tconst", (funct_t) tconst, (funct_t) test_tconst, 0, "! ~ & ^ | + << >>", 2, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"bitNand", (funct_t) bitNand, (funct_t) test_bitNand, 2, "~ |", 6, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"ezOverflow", (funct_t) ezOverflow, (funct_t) test_ezOverflow, 2,
    "! ~ & ^ | + << >>", 6, 2,
  {{0, TMax},{0,TMax},{TMin,TMax}}},

 {"fastModulo", (funct_t) fastModulo, (funct_t) test_fastModulo, 2, "! ~ & ^ | + << >>", 4, 3,
  {{0, TMax},{1,31},{TMin,TMax}}},

 {"findDifference", (funct_t) findDifference, (funct_t) test_findDifference, 2,
    "~ & |", 9, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"absVal", (funct_t) absVal, (funct_t) test_absVal, 1,
    "! ~ & ^ | + << >>", 6, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"secondLowBit", (funct_t) secondLowBit, (funct_t) test_secondLowBit, 1,
    "! ~ & ^ | + << >>", 8, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"byteSwap", (funct_t) byteSwap, (funct_t) test_byteSwap, 3, "! ~ & ^ | + << >>", 20, 5,
  {{TMin, TMax},{0,3},{0,3}}},

 {"byteCheck", (funct_t) byteCheck, (funct_t) test_byteCheck, 1, "! ~ & ^ | + << >>", 24, 5,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"fractions", (funct_t) fractions, (funct_t) test_fractions, 1, "! ~ & ^ | + << >>", 8, 5,
  {{0, 268435456},{TMin,TMax},{TMin,TMax}}},

 {"biggerOrEqual", (funct_t) biggerOrEqual, (funct_t) test_biggerOrEqual, 2,
    "! ~ & ^ | + << >>", 24, 6,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"hdOverflow", (funct_t) hdOverflow, (funct_t) test_hdOverflow, 2,
    "! ~ & ^ | + << >>", 20, 6,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"overflowCalc", (funct_t) overflowCalc, (funct_t) test_overflowCalc, 3,
    "! ~ & ^ | + << >>", 30, 7,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"logicalShift", (funct_t) logicalShift, (funct_t) test_logicalShift, 2,
    "! ~ & ^ | + << >>", 20, 8,
  {{TMin, TMax},{0,31},{TMin,TMax}}},

 {"partialFill", (funct_t) partialFill, (funct_t) test_partialFill, 2,
    "! ~ & ^ | + << >>", 24, 8,
  {{0, 15},{16,31},{TMin,TMax}}},


//float
 {"float_abs", (funct_t) float_abs, (funct_t) test_float_abs, 1,
    "$", 20, 3,
     {{1, 1},{1,1},{1,1}}},
 {"float_cmp", (funct_t) float_cmp, (funct_t) test_float_cmp, 2,
    "$", 40, 5,
     {{1, 1},{1,1},{1,1}}},
 {"float_pow2", (funct_t) float_pow2, (funct_t) test_float_pow2, 2,
    "$", 50, 6,
     {{1, 1},{0,255},{1,1}}},
 {"float_i2f", (funct_t) float_i2f, (funct_t) test_float_i2f, 1,
    "$", 40, 7,
     {{1, 1},{1,1},{1,1}}},



 {"oddParity", (funct_t) oddParity, (funct_t) test_oddParity, 1, "! ~ & ^ | + << >>", 56, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"bitReverse", (funct_t) bitReverse, (funct_t) test_bitReverse, 1, "! ~ & ^ | + << >>", 56, 2,
  {{1, TMax},{TMin,TMax},{TMin,TMax}}},

 {"mod7", (funct_t) mod7, (funct_t) test_mod7, 1, "! ~ & ^ | + << >>", 100, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},


  {"", NULL, NULL, 0, "", 0, 0,
   {{0, 0},{0,0},{0,0}}}
};