/* clz8.c -- count leading zeroes in bytes */

/* first attempt - use a 256-entry table */
static int clz8_1(unsigned int byte)
{
  static const unsigned char counts[256] =
  {
    8, 7, 6, 6, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
  };

  return counts[byte & 0xFF];
}

/* try a 16-entry table instead of 256 */
static int clz8_2(unsigned int byte)
{
  static const unsigned char counts[16] =
  {
    4, 3, 2, 2,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
  };

  int c;

  c = counts[byte >> 4];
  if (c == 4)
   c += counts[byte & 0xF];

  return c;
}

/* try it all in code, terminating as early as possible */
static int clz8_3(unsigned int byte)
{
  if (byte & 0x80) return 0; // can use & or >=
  if (byte & 0x40) return 1;
  if (byte & 0x20) return 2;
  if (byte & 0x10) return 3;
  if (byte & 0x08) return 4;
  if (byte & 0x04) return 5;
  if (byte & 0x02) return 6;
  if (byte & 0x01) return 7;
  return 8;
}

/* try splitting between code and data, noticing that half the table is zero,
 * return 0 if byte >= 128 */
static int clz8_4(unsigned int byte)
{
  static const unsigned char counts[128] =
  {
    8, 7, 6, 6, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
  };

  return byte >= 128 ? 0 : counts[byte];
}

/* try packing the table to see if that offers any benefit */
static int clz8_5(unsigned int byte)
{
#define M(a,b,c,d,e,f,g,h) \
  ((h << 28) | (g << 24) | (f << 20) | (e << 16) | (d << 12) | (c << 8) | (b << 4) | (a << 0))

  static const unsigned int counts[128 >> 3] =
  {
    M(8, 7, 6, 6, 5, 5, 5, 5),
    M(4, 4, 4, 4, 4, 4, 4, 4),
    M(3, 3, 3, 3, 3, 3, 3, 3),
    M(3, 3, 3, 3, 3, 3, 3, 3),
    M(2, 2, 2, 2, 2, 2, 2, 2),
    M(2, 2, 2, 2, 2, 2, 2, 2),
    M(2, 2, 2, 2, 2, 2, 2, 2),
    M(2, 2, 2, 2, 2, 2, 2, 2),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
    M(1, 1, 1, 1, 1, 1, 1, 1),
  };

  return byte >= 128 ? 0 : (counts[byte >> 3] >> (byte & 0x7) * 4) & 0xf;

#undef M
}

/* gerph's first attempt: halve at each step */
static int clz8_6(unsigned int byte)
{
  int c = 0;

  if ((byte & 0xF0) == 0)
  {
    c += 4;
    byte <<= 4;
  }

  if ((byte & 0xC0) == 0)
  {
    c += 2;
    byte <<= 2;
  }

  if ((byte & 0x80) == 0)
  {
    c += 1;
    byte <<= 1;
  }

  if ((byte & 0x80) == 0)
  {
    c += 1;
  }

  return c;
}

/* a version which assumes that bytes with fewer leading zeroes are more
 * common. terminates as early as possible. */
static int clz8_7(unsigned int byte)
{
  static const unsigned char counts[32] =
  {
    8, 7, 6, 6, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
  };

  if (byte & 0x80) return 0;
  if (byte & 0x40) return 1;
  if (byte & 0x20) return 2;

  return counts[byte];
}

/* gerph's variant of clz8_7 which is smaller but later terminating */
static int clz8_8(unsigned int byte)
{
  static const unsigned char counts[32] =
  {
    8, 7, 6, 6, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
  };

  int c = 0;

  if (byte < 0x80) c++;
  if (byte < 0x40) c++;
  if (byte < 0x20) c = counts[byte];

  return c;
}

// gerph's cunning idea
//
// ; => r0 = number to count leading zeros (0-255)
// ; <= r0 = number of leading zeros
//     SUB     r1, r0, #128            ; top bit = 0 if > 128
//                                     ; top bit = 1 if < 128
//     MOV     r0, r1, LSR #31
//     CMP     r1, #-64
//     ADDLS   r0, r0, #1               ; these LS codes ought to be LT
//     CMP     r1, #-96
//     ADRLS   r0, table+128
//     LDRLSB  r0, [r0, r1]
//     MOV     pc, lr
// table
//     =       8, 7, 6, 6, 5, 5, 5, 5,
//     =       4, 4, 4, 4, 4, 4, 4, 4,
//     =       3, 3, 3, 3, 3, 3, 3, 3,
//     =       3, 3, 3, 3, 3, 3, 3, 3,
//
/* my C recreation of it: */
static int clz8_9(unsigned int byte)
{
    static const unsigned char counts[32] =
    {
        8, 7, 6, 6, 5, 5, 5, 5,
        4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
    };

    int biased;
    int c;

    biased = byte - 128;
    c = (unsigned int) biased >> 31; /* bit 7, 0/1 becomes bit 0, 1/0 */
    if (biased < -64)
        c++;
    if (biased < -96)
        c = counts[biased + 128];

    return c;
}

#include <stdio.h>

#define NROUTINES 9

int main(void)
{
  int i;

  for (i = 0; i < 256; i++)
  {
    int r[NROUTINES];

    r[0] = clz8_1(i);
    r[1] = clz8_2(i);
    r[2] = clz8_3(i);
    r[3] = clz8_4(i);
    r[4] = clz8_5(i);
    r[5] = clz8_6(i);
    r[6] = clz8_7(i);
    r[7] = clz8_8(i);
    r[8] = clz8_9(i);

    if (r[0] != r[1] ||
        r[0] != r[2] ||
        r[0] != r[3] ||
        r[0] != r[4] ||
        r[0] != r[5] ||
        r[0] != r[6] ||
        r[0] != r[7] ||
        r[0] != r[8])
      printf("diff: %d: %d %d %d %d %d %d %d %d %d\n", i, r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8]);
  }

  return 0;
}
