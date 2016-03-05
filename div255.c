/* div255.c
 *
 * Exploring various different ways to divide by 255 for values up to 65535.
 *
 * David Thomas, March 2016
 */

#include <stdio.h>
#include <string.h>

/* (A) This version I was using and discovered it to be BROKEN for 65280 and 65535. */
#define DIV_255_A(x) ((((x) << 8) + (x) + 255) >> 16)

/* (B) From the Blinn article. Goes wrong at 65790. */
#define DIV_255_B(x) (((x) + 0x01 + (((x) + 0x01) >> 8)) >> 8)

/* (rB) As above but with built-in rounding. I saw this used in an Xorg patch. */
#define DIVROUND_255_B(x) (((x) + 0x80 + (((x) + 0x80) >> 8)) >> 8)

/* (C) Another Blinn one. Like above it goes wrong at 65790. */
#define DIV_255_C(x) ((x) * 257 + 257) >> 16

/* (D) Reciprocal multiplication -- uses multiply + long long -- gcc and clang will typically generate this. */
#define DIV_255_D(x) (((x) * 0x80808081ULL) >> 39)

#define MAXWAYS 5

int main(void)
{
    int nwrong[MAXWAYS];
    int i;

    memset(nwrong, 0, sizeof(nwrong));

    for (i = 0; i < 65536; i++)
    {
        int reference;
        int reference_rounded;
        int result;

        reference = i / 255;
        reference_rounded = (i + 127) / 255;

        result = DIV_255_A(i);
        if (result != reference)
        {
            nwrong[0]++;
            printf("A @ %d: %d <> %d\n", i, result, reference);
        }

        result = DIV_255_B(i);
        if (result != reference)
        {
            nwrong[1]++;
            printf("B @ %d: %d <> %d\n", i, result, reference);
        }

        result = DIVROUND_255_B(i);
        if (result != reference_rounded)
        {
            nwrong[2]++;
            printf("rB @ %d: %d <> %d\n", i, result, reference_rounded);
        }

        result = DIV_255_C(i);
        if (result != reference)
        {
            nwrong[3]++;
            printf("C @ %d: %d <> %d\n", i, result, reference);
        }

        result = DIV_255_D(i);
        if (result != reference)
        {
            nwrong[4]++;
            printf("D @ %d: %d <> %d\n", i, result, reference);
        }
    }

    for (i = 0; i < MAXWAYS; i++)
        if (nwrong[i] > 0)
            printf("(%c) has %d errors\n", 'A' + i, nwrong[i]);
}
