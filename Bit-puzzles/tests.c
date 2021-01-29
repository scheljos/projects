/* Testing Code */

#include <limits.h>
#include <math.h>

/* Routines used by floation point test code */

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

int test_bitMatch(int x, int y)
{
    int i;
    int result = 0;
    for (i = 0; i < 32; i++) {
 int mask = 1 << i;
 int bit = (x & mask) == (y & mask);
 result |= bit << i;
    }
    return result;
}
int test_magicNumber(void) {
    return 0xfff2ffff;
}
void *memset(void *s, int c, unsigned long n);
int test_copyLSByte(int x)
{
  unsigned char low_byte = x;
  int ret;
  memset(&ret, low_byte, sizeof(int));
  return ret;
}
int test_isNonNegative(int x) {
  return x >= 0;
}
int test_isLowercase(int x) {
  return (0x61 <= x) && (x <= 0x7A);
}
int test_isUppercaseConsonant(int x) {
  if (x >= 'A' && x <= 'Z') {
    if (x == 'A' || x == 'E' || x == 'I' || x == 'O' || x == 'U')
      return 0;
    else
      return 1;
  } else {
    return 0;
  }
}
int test_satUMul3(int x)
{
  unsigned ux = x;
  if ((ux+ux+ux)/3 != ux)
    return 0xFFFFFFFF;
  else
    return 3*ux;
}
unsigned test_floatUChar2Float(unsigned char x) {
  float f = (float) x;
  return f2u(f);
}
