#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void stk2pool(char st[80], char pol[4][8]);

int findop(char op[]);

int op2code(char nmb, int dis, char pol[4][8], char code[4]);

int findreg(char reg[8]);

int findsegr(char reg[8]);

int findsym(char sym[8]);

int str2num(char str[8]);

int findch(char c);

int step(int a, int b);

int offset(int a, char *str);

#endif
