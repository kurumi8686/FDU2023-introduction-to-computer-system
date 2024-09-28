/* 
 * CS:APP Data Lab 
 * 
 * name:谢志康  userid:kurumi
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;   //x向左进位，即*2
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
#include "bits.h"

//P1
/* 
 * tconst - return a constant value 0xFFFFFFE0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int tconst(void)
  {
      return ~31;
  }

//P2
/* 
 * bitNand - return ~(x&y) using only ~ and | 
 *   Example: bitNand(4, 5) = -5, bitNand(3, 11) = -4
 *   Legal ops: ~ |
 *   Max ops: 6
 *   Rating: 2
 */
int bitNand(int x, int y) {

      return (~x) | (~y);
  }

//P3
/*
 * ezOverflow - determine if the addition of two signed positive numbers overflows,
 *      and return 1 if overflowing happens and 0 otherwise
 *   You can assume 0 <= x, 0 <= y
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int ezOverflow(int x,int y) {
    int t = x + y;
    int i = ~(t >> 31) + 1;
    return i;
}

//P4
/* 
 * fastModulo - return x%(2^y)   //这是指数，不是异或
 *   You can assume 0 <= x, 1 <= y <= 31
 *   Example: fastModulo(0x1234567,0xA) = 0x167
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 3
 */
int fastModulo(int x,int y) {
    int z = 1 << y;
    int t = ~0;
    return x & (z + t);
}


//P5
/*
 * findDifference - return a mask that marks the different bits of y compared to x
 *   Example: findDifference(12, 10) = findDifference(10, 12) = 6
 *   Legal ops: ~ & |
 *   Max ops: 9
 *   Rating: 3
 */
int findDifference(int x,int y) {
	return (x&~y)|(~x&y);
}

//P6
/*
 * absVal - return the absolute value of x
 *   Examples: absVal(-10) = 10
 *			   absVal(5) = 5
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 4
 */
int absVal(int x) {
    int t = x;
    int i = t >> 31;  //正数是0，负数是-1
    int j = (i ^ t) + ~i + 1;
    return j;
}

//P7
/*
 * secondLowBit - return a mask that marks the position of the second least significant 1 bit.
 *   Examples: secondLowBit(0x00000110) = 0x00000100
 *			   secondLowBit(0xFEDC1A80) = 0x00000200
 *             secondLowBit(0) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 4
 */
int secondLowBit(int x) 
{
    //找最低位的1
    int y = x & (~x + 1);
    x = x ^ y;
    return x & (~x + 1);
}

//P8
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 20
 *  Rating: 5
 */
int byteSwap(int x, int n, int m)
{
    int y1 = (x >> (n << 3)) & 0xFF;
    int y2 = (x >> (m << 3)) & 0xFF;  //只取得那个byte上的数即可
    x = x & (~(0xFF << (n << 3)));
    x = x & (~(0xFF << (m << 3)));
    x = x | y1 << (m << 3);
    x = x | y2 << (n << 3);
    return x;
}

//P9
/* 
 * byteCheck - Returns the number of bytes that are not equal to 0
 *   Examples: byteCheck(0xFE0A4100) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 5
 */
int byteCheck(int x) {
    int i = 0xFF;
    int t = x;
    //取出四个byte分别的内容
    int by = 8;
    int a = t & i;
    int b = t & (i << by);
    int c = t & (i << (by + by));
    int d = t & (i << (by + by + by));
    int e = 0;
    a = !a; a = !a;//if a!=0;then a==1;
    b = !b; b = !b;
    c = !c; c = !c;
    d = !d; d = !d;
    e = e + a + b + c + d;
    return e;
}
//P10
/* 
 * fractions - return floor(x*7/16), for 0 <= x <= (1 << 28), x is an integer 
 *   Example: fractions(20) = 8
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 5
 */
int fractions(int x) {
    int t = x;
    x = x << 3;
    x = x + (~t + 1);
    x = x >> 4;
    return x;
}
//P11
/* 
 * biggerOrEqual - if x >= y  then return 1, else return 0 
 *   Example: biggerOrEqual(4,5) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 6
 */
int biggerOrEqual(int x, int y) {
    int t = x + (~y + 1);//x-y
    int a = !(!(x >> 31));//sign x，非负是0，负数为1
    int b = !(!(y >> 31));//sign y，非负是0，负数为1  
    int both = !(a ^ b);//符号相同为1，相异为0
    int i = t >> 31;//非负是0，负数-1
    int j = !i;//非负是1，负数0
    int res = (j & both) | ((!a) & b);//符号位相同且x>=y 或 符号位不同且x>=0;y<0
    return res;
}

//P12
/*
 * hdOverflow - determine if the addition of two signed 32-bit integers overflows,
 *      and return 1 if overflowing happens and 0 otherwise
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 6
 */
int hdOverflow(int x,int y) {
    int t = x + y;
    int signt = !(t >> 31);  //非负为1，负数为0
    int signx = !(x >> 31);
    int signy = !(y >> 31);
    int res = (signx & signy & (!signt)) | ((!signx) & (!signy) & signt);  //正+正=负,负+负=正
	return res;
}

//P13
/* 
 * overflowCalc - given binary representations of three 32-bit positive numbers and add them together, 
 *      return the binary representation of the part where bits are higher than 32.
 *   Examples: overflowCalc(0xffffffff,0xffffffff,0xffffffff) = 2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 7
 */
int overflowCalc(int x, int y, int z) {
    int i = 0xF;
    int a1 = i & x;
    int a2 = i & y;
    int a3 = i & z;
    int j;
    int t = a1 + a2 + a3;
    t = t >> 4;
    j = ~(i << 28);
    x = (x >> 4) & j;
    y = (y >> 4) & j;
    z = (z >> 4) & j;
    return (x + y + z + t) >> 28;
}

//P14
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 8
 */
int logicalShift(int x, int n) {
    int i, j, res;
    x = x >> n;
    j = ~(1 << n) + 1;
    j = ~j;
    i = j << (32 + (~n+1));
    i = ~i;
    res = x & i;
    return res;
}

//P15
/* 
 * partialFill - given l, h, fill all even digits within the [l, h] interval with 1 (subscripts starting from 0)
 *   Can assume that 0 <= l <= 15, 16 <= h <= 31
 *   Examples: partialFill(13,16) = 0x00014000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 8
 */
int partialFill(int l, int h) {
    int a, b, i, j, t, res;
    a = 85; // 01010101
    b = a + (a << 8) + (a << 16) + (a << 24);
    i = 1 << h;
    i = i << 1;
    i = ~i + 1;
    j = 1 << l;
    j = ~j + 1;
    j = ~j;
    t = i + j;
    t = ~t; // [l-h]全为1，其余全为0
    res = b & t;
    return res;
}

//P16
/* 
 * float_abs - Return bit-level equivalent of expression |f| (absolute value of f) for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 20
 *   Rating: 3
 */
unsigned float_abs(unsigned uf) {
    unsigned i, j, t, exp, rem;
    int k;
    //0x80000000  sign_mask符号位
    //0x7F800000  exponent_mask指数位
    if (!(uf >> 31)) return uf; //正数 
    i = 1;
    i = i << 31;//0x800000000
    j = 255;
    j = j << 23;//0x7F800000
    t = ~(j + i);
    exp = j & uf; //取出指数位
    rem = t & uf; //取出小数位
    k = uf;
    if (!(exp + (~j + 1)) && rem) //rem!=0
    {
        return k;
    }
    return uf & (~i);//将符号位剔除，取正数
}

//P17
/*
 * float_cmp - Return 1 if uf1 > uf2, and 0 otherwise.
 *   Both of the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When any of the arguments is NaN, return 0.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 40
 *   Rating: 5
 */

unsigned float_cmp(unsigned uf1, unsigned uf2) {
    unsigned sign1 = uf1 >> 31; // uf1的符号位
    unsigned sign2 = uf2 >> 31; // uf2的符号位
    unsigned i = ~(1 << 31);
    unsigned j = 255 << 23;
    uf1 = uf1 & i;
    uf2 = uf2 & i;
    if (uf1 > j || uf2 > j) return 0; // 检查是否为NaN
    if (uf1 == uf2) {
        if (uf1 == 0 || sign1 == sign2) return 0;
        return sign1 < sign2 ? 1 : 0;
    }
    if (sign1 == sign2)
    {
        return uf1 > uf2 ? sign1 == 0 : sign1 == 1;
    }
    return sign1 < sign2 ? 1 : 0; // 符号不相等的情况
}



//P18
/* 
 * float_pow2 - Return bit-level equivalent of expression f*(2^n) for
 *   floating point argument f and integer argument n.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   You can assume 0 <= n <= 255
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 50
 *   Rating: 6
 */

unsigned float_pow2(unsigned uf, int n) {
    unsigned i, j, t, exp, num, sign;
    i = 1 << 31;//0x800000000，符号部分
    j = 255 << 23;//0x7F800000，指数部分
    t = ~(i + j);//余数部分
    sign = i & uf;
    exp = (j & uf) >> 23;
    num = t & uf;
    //特殊情况
    if (uf == 0 || uf == i || n == 0 || (exp == 255 && num != 0)) return uf;
    if (exp == 0) //0.0000……
    {
        while (n > 0 && num <= t)
        {
            n--;
            num <<= 1;
        }
        if (n == 0) return sign + num;
        else
        {
            if (((num & j) >> 23) + n >= 255) return sign + j;//无穷大
            return sign + num + (n << 23);//n加在指数位 
        }
    }
    else //1.0000…… 
    {
        exp += n;
        if (exp >= 255)
        {
            exp = 255;
            num = 0; //or it will be nan
        }
        return sign + (exp << 23) + num;
    }
}

//P19
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 40
 *   Rating: 7
 */

unsigned float_i2f(int x) {
    unsigned num, t, lownum, markflow, d;
    int j = 1 << 31;
    unsigned sign = x & j;//取出符号位 
    unsigned exp = 127;//偏移量 
    int count = 32, i;

    //特判 
    if (x == 0) return x;
    //x是int类型，只需要保留正数进行后续处理即可 
    if (sign) x = ~x + 1;
    //找到最高位的1
    while (j)
    {
        count--;
        if (j & x) break;
        j >>= 1;
    }
    //最高位的1的位置赋值给exp（相当于提项） 
    exp += count;
    i = count - 23;

    t = ~(1 << count); //count+1位 
    num = x & t;
    lownum = 1 << i; //num最低位 
    markflow = lownum >> 1; //记录溢出 
    //处理溢出 
    if (i > 0)
    {
        d = num & (lownum - 1);
        if ((d > markflow) | (d == markflow && (num & lownum)))
        {
            num += lownum;
            if (num > 0x3fffffff)
            {
                num = 0;
                exp++;
            }
        }
        num >>= i;
    }
    else num <<= (-i);  //i小于0 
    return sign | exp << 23 | num;
}


#ifdef NOT_SUPPOSED_TO_BE_DEFINED
#   __          __  _
#   \ \        / / | |                         
#    \ \  /\  / /__| | ___ ___  _ __ ___   ___ 
#     \ \/  \/ / _ \ |/ __/ _ \| '_ ' _ \ / _ \
#      \  /\  /  __/ | (_| (_) | | | | | |  __/
#       \/  \/ \___|_|\___\___/|_| |_| |_|\___|
#                                              

    #  ██╗  ██╗ ██████╗ ███╗   ██╗ ██████╗ ██████╗     ██████╗  █████╗ ██████╗ ████████╗
    #  ██║  ██║ ██╔══██╗████╗  ██║██╔═██╗██╔══██╗    ██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝
    #  ██████ ║██║   ██║██╔██╗ ██║██║   ██║██████╔╝    ██████╔╝███████║██████╔╝   ██║
    #  ██╔═██║ ██║   ██║██║╚██╗██║██║   ██║██╔██╗      ██╔═══╝ ██╔══██║██╔══██╗   ██║
    #  ██║  ██╚██████╔╝██║ ╚████║╚██████╔██║   ██║    ██║     ██║  ██║██║  ██║   ██║
    #  ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝    ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝
#                                                                                   
#endif

//P20
/*
 * oddParity - return the odd parity bit of x, that is,
 *   when the number of 1s in the binary representation of x is even, then the return 1, otherwise return 0.
 *   Examples:oddParity(5) = 1, oddParity(7) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 56
 *   Challenge: You will get 1 extra point if you use less than or equal to 34 ops
 *   Rating: 2
 */
 int oddParity(int x) {
    int m1, m2, m3, m4, m5, mask1, mask2, mask3, mask4, mask5, count;
    m1 = 0x55 + (0x55 << 8); mask1 = m1 + (m1 << 16);// 0101 0101 0101 0101 0101 0101 0101 0101
    m2 = 0x33 + (0x33 << 8); mask2 = m2 + (m2 << 16);// 0011 0011 0011 0011 0011 0011 0011 0011
    m3 = 0x0f + (0x0f << 8); mask3 = m3 + (m3 << 16); // 0000 1111 0000 1111 0000 1111 0000 1111
    m4 = 0xff; mask4 = m4 + (m4 << 16);// 0000 0000 1111 1111 0000 0000 1111 1111
    m5 = 0xff; mask5 = m5 + (m5 << 8);// 0000 0000 0000 0000 1111 1111 1111 1111

    count = (x & mask1) + ((x >> 1) & mask1);
    count = (count & mask2) + ((count >> 2) & mask2);
    count = (count & mask3) + ((count >> 4) & mask3);
    count = (count & mask4) + ((count >> 8) & mask4);
    count = (count & mask5) + ((count >> 16) & mask5);

    return !(count & 1);
}

//P21
/*
 * bitReverse - Reverse bits in an 32-bit integer
 *   Examples: bitReverse(0x80000004) = 0x20000001
 *             bitReverse(0x7FFFFFFF) = 0xFFFFFFFE
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 56
 *   Challenge: You will get 1 extra point if you use less than or equal to 34 ops
 *   Rating: 2
 */
 int bitReverse(int x) {
    int m1, m2, m3, m4, m5, mask1, mask2, mask3, mask4, mask5;
    m1 = 0x55 + (0x55 << 8); mask1 = m1 + (m1 << 16);// 0101 0101 0101 0101 0101 0101 0101 0101
    m2 = 0x33 + (0x33 << 8); mask2 = m2 + (m2 << 16);// 0011 0011 0011 0011 0011 0011 0011 0011
    m3 = 0x0f + (0x0f << 8); mask3 = m3 + (m3 << 16); // 0000 1111 0000 1111 0000 1111 0000 1111
    m4 = 0xff; mask4 = m4 + (m4 << 16);// 0000 0000 1111 1111 0000 0000 1111 1111
    m5 = 0xff; mask5 = m5 + (m5 << 8);// 0000 0000 0000 0000 1111 1111 1111 1111
    x = ((x >> 16) & mask5) | (x << 16);
    x = ((x >> 8) & mask4) | ((x & mask4) << 8);
    x = ((x >> 4) & mask3) | ((x & mask3) << 4);
    x = ((x >> 2) & mask2) | ((x & mask2) << 2);
    x = ((x >> 1) & mask1) | ((x & mask1) << 1);
    return x;
}

//P22
/*
 * mod7 - calculate x mod 7 without using %.
 *   Example: mod7(99) = 1
 *            mod7(-101) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 100
 *   Challenge: You will get 2 extra point if you use less than or equal to 56 ops
 *   Rating: 2
 */
 int mod7(int x) {
    int mask = 0x3f;
    int count = 0;
    int sign = 1 << 31;
    int temp;
    sign = sign & x;
    sign = sign >> 31;//po0,ne1
    x = (sign & (~x + 1)) | (~sign & x);
    count = count + (mask & x);
    x = x >> 6;
    count = count + (mask & x);
    x = x >> 6;
    count = count + (mask & x);
    x = x >> 6;
    count = count + (mask & x);
    x = x >> 6;
    count = count + (mask & x);
    x = x >> 6;
    count = count + (3 & x);
    //考虑到count已经是一个不大的数，测试发现绝不会大于1024，于是直接解。
    count = (count & 7) + ((count >> 3) & 7) + ((count >> 6) & 7);
    count = (count & 7) + ((count >> 3) & 7);
    count = (count & 7) + ((count >> 3) & 7);
    //特判等于7的情况
    temp = !(count + (~7 + 1));
    temp = ~temp + 1;//如果count==7,temp==-1,otherwise temp==0.
    count = ~temp & count;
    return (sign & (~count + 1)) | (~sign & count);
}
