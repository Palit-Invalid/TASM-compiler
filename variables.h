#ifndef VARIABLES_H
#define VARIABLES_H

static char npr, cnt, stk[80], pole[4][8];
static int psym, pmdf, i;


//CBW, JE, ADC, SAL, INC
static const char optab[14][5] = 
{
    "mov", "push", "cmp", "jle", 
    "nop", "xor", "ror", "dw", 
    "dec", "lahf", "je", "shr", 
    "or", "int"
};

static const char regtab[8][3] = 
{
    "ax", "cx", "dx", "bx",
    "sp", "bp", "si", "di"
};

static const char segrtab[4][3] =
{
    "es", "cs", "ss", "ds"
};

static const char hextab[17] = "0123456789abcdef";

static char modif[20];

static struct segt
{
    char name[8];
    int len;
} segtab[2];

static struct symt
{
    char name[8];
    char sgm;
    int dsp;
} symtab[10];

static const char fname[2][10] =
{
    "t1.dat", "t2.dat"
};

#endif
