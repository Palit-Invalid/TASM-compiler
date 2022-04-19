#include<stdio.h>

#include "variables.h"

#ifndef GEN_OBJ_H
#define GEN_OBJ_H

int firstBlock(FILE *f, char *compiler, char *filename);
int secondBlock(FILE *f, int num_segments, struct segt *segtab);
int thirdBlock(FILE *f); //segments
int fourthBlock(FILE *f); //ending

#endif
