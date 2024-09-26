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
//1
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  return ~(~x & ~y);
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
  /* Nota: la idea es ir haciendo un XOR entre las mitades de <x>
   * Es decir, vamos comparando las mitades de x entre sí, teniendo en cuenta que:
   * si tenemos una cantidad par de unos -> gracias al XOR se cancelan todos los unos
   * si tenemos una cantidad impar de unos -> habrá algún uno que no se va a cancelar y se irá arrastrando
   */
  int x1 = x ^ (x >> 16);
  int x2 = x1 ^ (x1 >> 8);
  int x3 = x2 ^ (x2 >> 4);
  int x4 = x3 ^ (x3 >> 2);
  return ((x4 ^ (x4>>1)) & 1);
}
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return (~x & ~y);
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~(x & ~y) & ~(~x & y));
}
//2
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
  int salida = 0x55; // salida = 0000 (...)  0101 0101
  salida = (salida << 8) + 0x55; // salida = 0000 (...) 0101 0101 0101 0101
  salida = (salida << 8) + 0x55; 
  salida = (salida << 8) + 0x55;
  return salida;//1010101
}
/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) { 
    int maskOddBit = 0xAA;
    maskOddBit = (maskOddBit << 8) + 0xAA; 
    maskOddBit = (maskOddBit << 8) + 0xAA; 
    maskOddBit = (maskOddBit << 8) + 0xAA; //maskOddBit = 1010 1010 1010 1010
    int oddBit = x & maskOddBit; //Se queda con '1' si en alguna pos.Impar había un '1'
    return !!oddBit;//(!oddBit)=> Si es oddBit = 0 devuelve = 1 y  si oddBit != 0 => (!oddBit) = 0, entonces se niega 2 veces para la consigna. 
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
    //creo mascara para el primer byte
    int maskByte0 = ~(~0x00 << 8);

    //encuentro las mascaras para los bits del swap
    int maskByteM = maskByte0 << (m << 3);
    int maskByteN =  maskByte0 << (n << 3);

    //Extraigo bytes n y m y los posiciono en 0 y elimino posibles replicados del msb !0
    int byteM = ((x & maskByteM) >> (m << 3)) & maskByte0;
    int byteN = ((x & maskByteN) >> (n << 3)) & maskByte0;

    //conservo el resto de posiciones que no hacen swap
    int maskRestantes = ~(maskByteM | maskByteN);
    int bytesRestantes = x & maskRestantes;

    //colocar bytes extraidos en sus nuevas posiciones
    int nuevoByteM = byteM << (n << 3);
    int nuevoByteN = byteN << (m << 3);
    return nuevoByteM | nuevoByteN | bytesRestantes;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int aux_x = x >> (n + (~0x1 + 1)); // x >> (n - 1)
  int signo_x = x >> 31; 
  return !((aux_x ^ signo_x)); 
}
/* 
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int oddBits(void) {

  int resultado = 0xAA;  //   resultado = 0000 (...) 1010 1010
  resultado = (resultado << 8) + 0xAA; //resultado = 0000 (...) 1010 1010 1010 1010
  resultado = (resultado << 8) + 0xAA; 
  resultado = (resultado << 8) + 0xAA;
  return resultado;//1010 1010
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
    //replico el bit de signo en todos los bytes
    int bitSign = x >> 31;
    //esCero==1 si x != 0 y esCero==0 si x = 0
    int esCero = !!x;
    return bitSign | esCero;
}
//3
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  /* Sabiendo que hay overflow sii: x e y tienen el mismo signo e (x+y) tiene signo opuesto: */
  int msb_x = x >> 31; // Guardo el signo de x
  int msb_y = y >> 31; // Guardo el signo de y
  int msb_sum = (x + y) >> 31; // Guardo si la suma es positiva o negativa

  int v_pos = (!msb_x & !msb_y) & (msb_sum); // Averiguo si el resultado de dos positivos dió un valor negativo
  int v_neg = (msb_x & msb_y) & (!msb_sum); // Averiguo si al sumar dos nros negativos el resultado dió positivo

  return !(v_pos | v_neg); // Si no hubo ni v_pos ni v_neg, entonces no hay overflow
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  int maskOne = ~0; //1111 (...) 1111
  //Todos los bits son 1's hasta highMask y 0's en los bit superiores
  int highMask = (maskOne << highbit) << 1;
  //Todos los bits son 1's hasta lowbit y 0's en los bits inferiores
  int lowMask = maskOne << lowbit;
  
  //Uso XOR para obtener los bits que son diferentes entre highMask y lowMask, devolverá '1' donde sean distintos
  //Usando lowMask aseguro que los bits de menor orden se mantengan, y cumple lowbit > highbit
  return (highMask ^ lowMask) & lowMask;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // nota: !x -> 1 si x == 0 else ->0
  // luego, si x es cero maskCondicional es todos 1, 
  //y si no es cero maskCondicional es todos ceros
  int maskCondicional = ~(!x) + 1;
  return (maskCondicional & z) | (~maskCondicional & y);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // Declaro algunas variables:
  int nibble1, nibble2, nibble3, nibble4, nibble5, nibble6, nibble7, nibble8;
  int res,sum, mask_nibble;

  // Genero una mascara que separe a <x> en baches de 4 bits:
  int mask = 0x11;
  mask = (mask << 8) + 0x11; 
  mask = (mask << 8) + 0x11; 
  mask = (mask << 8) + 0x11; // mask = 0001 0001 (...) 0001

  // Armo 8 baches de 4 bits para contar la cantidad de bits en 1:
  int sum1 = (x) & mask;
  int sum2 = (x >> 1) & mask;
  int sum3 = (x >> 2) & mask;
  int sum4 = (x >> 3) & mask;
  int sum5 = (x >> 4) & mask;
  int sum6 = (x >> 5) & mask;
  int sum7 = (x >> 6) & mask;
  int sum8 = (x >> 7) & mask;
  
  // Cada nibble representa la cantidad de bits en 1 en cada bache de <x>:
  sum = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8; 
  // Es decir: si sum = 0000 0000 0000 0000 0000 0010 0011 0001 ->
  // -> Los primeros 4 bits menos significativos tienen 0001 == 1 bit en 1
  // -> Los segundos 4 bits menos significativos tienen 0011 == 3 bits en 1 (...)
  
  // Ahora obtengo cada nibble por separado:
  mask_nibble = 0xF;
  nibble1 = sum & mask_nibble;
  nibble2 = (sum >> 4) & mask_nibble;
  nibble3 = (sum >> 8) & mask_nibble;
  nibble4 = (sum >> 12) & mask_nibble;
  nibble5 = (sum >> 16) & mask_nibble;
  nibble6 = (sum >> 20) & mask_nibble;
  nibble7 = (sum >> 24) & mask_nibble;
  nibble8 = (sum >> 28) & mask_nibble;
  
  // Sumo cada nibble:
  res = nibble1 + nibble2 + nibble3 + nibble4 + nibble5 + nibble6 + nibble7 + nibble8;
  return (res); 
}
/* 
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and & 
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
  //Obtengo los bits que son 1 en ambos números
  int maskOne = x & y;
  //Obtengo los bits que son 0 en ambos números
  int maskZero = ~x & ~y;
  //Obtenemos los bits que coinciden y l niego para mantener 1 en las posiciones donde los bits de x y y coinciden
  int match = ~(~maskOne & ~maskZero);
  return match;
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
  int maskByte0 = ~(~0x00 << 8);
  int maskByteN = maskByte0 << (n << 3);
  int bytesRestantes = x & ~maskByteN;
  int byteCReubicado = c << (n << 3);
  return byteCReubicado | bytesRestantes;
}
//4
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum negative value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
  return 2;
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x80000001) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
  return 2;
}
/* 
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
int isNonZero(int x) {
  //calcular el inverso artimetico, si es negX 
  int negX = ~x + 1;
  // asi nos aseguramos que haya un 1 en el bit mas significtivo si x no es cero
  int resSuma = negX | x;
  //si x era 0 su msb == 0 si sino msb=1
  return (resSuma >> 31) & 1;
}
/* 
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x187654321
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
  // Obtengo los n bits que irán al comienzo una vez que rotemos a x:
  int x_aux = x << (32 + (~n + 1)); 
  // Armo una máscara que me permita generar <n> ceros al comienzo de x. Para el ejemplo dado quedaría mask = 0x0FFFFFFF
  int mask = ~(~0x0 << (32 + (~n + 1)));
  // Genero n bits en 0 al comienzo de x para luego sumarle x_aux:
  x = (x >> n) & (mask); 
  return (x_aux + x);
}
//float
/* 
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {
  return 2;
}
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  unsigned int menosCero = 0x80000000; 
  //mascaras
  unsigned int maskFrac = 0x007FFFFF;
  unsigned int maskExp = 0x000000FF;
  //extraigo componentes del float uf
  unsigned int expF = (uf >> 23) & maskExp;
  unsigned int fracF = uf & maskFrac;
  //idem para ug
  unsigned int expG = (ug >> 23) & maskExp;
  unsigned int fracG = ug & maskFrac;

  int esNaNF = (expF == 255) && (fracF != 0);//0 -> true; else -> false
  int esNaNG = (expG == 255) && (fracG != 0);

  if (esNaNF || esNaNG ){
    return 0;
  }else if ((uf == menosCero || uf == 0) && (ug == menosCero || ug == 0)){
    return 1;
  }
  return uf == ug;
}
/* 
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
  // Verifico que <uf> no sea NaN: 
  // - Veo si <frac> es distinto o no de 000(...)0:
  int frac_mask = 0x007FFFFF; // Los 23 bits del frac están en 1 -> 0000 0000 0111 (...) 1111
  int frac_nan = (uf & (frac_mask)) != 0; // Si es NaN, <frac_nan> debe ser != 0000(...)0000

  // - Veo si <exp> es 111(...)111:
  int exp_mask = 0x7F800000; // Los 8 bits del exp están en 1 -> 0111 1111 1000 (...) 0000
  int exp_nan = (uf & (exp_mask)) == exp_mask; // Si la igualdad es 1 es porque el <exp> de <uf> es 111..1
  
  // Si <exp> = 111..1 y <frac> != 000.00 -> Es NaN
  if (exp_nan && frac_nan){
    return uf;
  }
  // Cambio el bit de signo:
  return uf ^ (0x1 << 31);
}
/* 
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
int floatIsLess(unsigned uf, unsigned ug) {
    return 2;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
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
int floatFloat2Int(unsigned uf) {
  return 2;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    return 2;
}
