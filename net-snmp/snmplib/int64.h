#ifndef INT64_INCLUDED
#define INT64_INCLUDED

typedef struct _u64 {
    unsigned int ulHi;     /* high 32 bits */
    unsigned int ulLo;     /* low 32 bits */
} U64;

void divBy10 __P((U64, U64 *, unsigned int *));
void multBy10 __P((U64, U64 *));
void incrByU16 __P((U64 *, unsigned int));
void zeroU64 __P((U64 *));
int isZeroU64 __P((U64 *));
char *printU64 __P((U64 *));

#endif
