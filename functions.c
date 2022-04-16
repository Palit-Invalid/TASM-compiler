#include "functions.h"
#include "variables.h"

#include<string.h>

void stk2pool(char st[80], char pol[4][8])
{
    int fl = 0, k = 0, j = 0, i = 0;
    while(st[i] != '\0' && st[i] != '\n')
    {
        if(st[i] == ' ' || st[i] == ',' || j == 8)
        {
            if(fl)
            {
                pol[k][j] = '\0';
                j = 0;
                k++;
                if(k == 4)
                    return;
                
                fl = 0;
            }
        }
        else if(st[i] == ';')
        {
            for(; k <= 3; k++)
            {
                pol[k][0] = '\0';              
            }
            return;  
        } else
        {
            fl = 1;
            pol[k][j] = st[i];
            j++;
        }
        i++;
    }
    pol[k][j] = '\0';
    k++;
    for(; k <= 3; k++)
    {
        pol[k][0] = '\0';
    }
    return;
}

int findop(char op[])
{
    for(i = 0; i <= 13; i++)
    {
        if(!strcmp(optab[i], op))
        return i + 1;
    }
    return 0;
}

int findsym(char sym[8])
{
    int i;
    for(i = 0; i <= psym; i++)
    {
        if(!strcmp(symtab[i].name, sym))
            return i;
    }
    return -1;
}

int findreg(char reg[8])
{
    int i;
    for(i = 0; i <= 7; i++)
    {
        if(!strcmp(regtab[i], reg))
            return i;
    }
    return -1;
}

int findsegr(char reg[8])
{
    int i;
    for(i = 0; i <= 3; i++)
    {
        if(!strcmp(segrtab[i], reg))
            return i;
    }
    return -1;
}

int op2code(char nmb, int dis, char pol[4][8], char code[4])
{
    struct bits
    {
        char b0 : 1; char b1 : 1; char b2 : 1; char b3 : 1;
        char b4 : 1; char b5 : 1; char b6 : 1; char b7 : 1;
    };

    union bytes
    {
        char c;
        struct bits b;
    } temp, bcode;

    union int2ch
    {
        char c[2];
        int d;
    } tmp2;

    int regn, regn2, segrn, tmp3;

    dis++;
    switch(nmb)
    {
        case 1: //MOV
            regn = findreg(pol[dis]);
            regn2 = findreg(pol[dis + 1]);
            segrn = findsegr(pol[dis]);
            if(!regn && !strcmp(segtab[0].name, pol[dis + 1]))
            {
                code[0] = 0xb8;
                code[1] = 0;
                code[2] = 0;
                if(npr)
                {
                    modif[pmdf] = cnt + 1;
                    pmdf++;
                    modif[pmdf] = 1;
                    pmdf++;
                }
                return 3;
            }
            if(segrn != -1 && regn2 != -1)
            {
                code[0] = 0x8e;
                bcode.c = 0xc0;
                temp.c = segrn;
                bcode.b.b3 = temp.b.b0;
                bcode.b.b4 = temp.b.b1;
                temp.c = regn2;
                bcode.b.b0 = temp.b.b0;
                bcode.b.b1 = temp.b.b1;
                bcode.b.b2 = temp.b.b2;
                code[1] = bcode.c;
                return 2;
            }
            if(regn != -1 || regn2 != -1)
            {
                if(!regn || regn2)
                {
                    bcode.c = 0xa1;
                    if(regn)
                    {
                        bcode.c = 0xa3;
                        regn = regn2;
                    }
                    else if(!strcmp(pol[dis + 1], "4C00h"))
                    {
                        code[0] = 0xb8;
                        code[1] = 0x00;
                        code[2] = 0x4c;
                        return 3;
                    }
                    else
                    {
                        dis++;
                    }
                    code[0] = bcode.c;
                    if(npr)
                    {
                        tmp3 = findsym(pole[dis]);
                        if(tmp3 == -1)
                            return 0;
                        else
                        {
                            tmp2.d = symtab[tmp3].dsp;
                            code[1] = tmp2.c[0];
                            code[2] = tmp2.c[1];
                        }
                        modif[pmdf] = cnt + 1;
                        pmdf++;
                        modif[pmdf] = symtab[tmp3].sgm;
                        pmdf++;
                    }
                    else
                    {
                        code[1] = 0;
                        code[2] = 0;
                    }
                    return 3;
                }
                else
                {
                    bcode.c = 0x8b;
                    dis++;
                    code[0] = bcode.c;
                    bcode.c = regn;
                    bcode.b.b3 = temp.b.b0;
                    bcode.b.b4 = temp.b.b1;
                    bcode.b.b5 = temp.b.b2;
                    code[1] = bcode.c;
                    if(npr)
                    {
                        tmp3 = findsym(pole[dis]);
                        if(tmp3 == -1)
                            return 0;
                        else
                        {
                            tmp2.d = symtab[tmp3].dsp;
                            code[2] = tmp2.c[0];
                            code[3] = tmp2.c[1];
                        }
                        modif[pmdf] = cnt + 2;
                        pmdf++;
                        modif[pmdf] = symtab[tmp3].sgm;
                        pmdf++;
                    } else
                    {
                        code[2] = 0;
                        code[3] = 0;
                    }
                    return 4;
                }
            }
            break;
        case 2: //PUSH
            regn = findreg(pol[dis]);
            segrn = findsegr(pol[dis]);
            if(regn != -1)
            {
                bcode.c = 0x50;
                temp.c = regn;
                bcode.b.b0 = temp.b.b0;
                bcode.b.b1 = temp.b.b1;
                bcode.b.b2 = temp.b.b2;
                code[0] = bcode.c;
                return 1;
            }
            if(segrn != -1)
            {
                bcode.c = 0x06;
                temp.c = segrn;
                bcode.b.b3 = temp.b.b0;
                bcode.b.b4 = temp.b.b1;
                code[0] = bcode.c;
                return 1;
            }
            else
            {
                return 0;
            }
            break;

        case 3: //CMP
            regn = findreg(pol[dis]);
            regn2 = findreg(pol[dis + 1]);
            if(regn != -1 && regn2 != -1)
            {
                code[0] = 0x3b;
                bcode.c = 0xc0;
                temp.c = regn;
                bcode.b.b3 = temp.b.b0;
                bcode.b.b4 = temp.b.b1;
                bcode.b.b5 = temp.b.b2;
                temp.c = regn2;
                bcode.b.b0 = temp.b.b0;
                bcode.b.b1 = temp.b.b1;
                bcode.b.b2 = temp.b.b2;
                code[1] = bcode.c;
                return 2;
            } else
            {
                return 0;
            }
            break;
        case 4: //JLE
            if(npr)
            {
                tmp3 = findsym(pole[dis]);
                if(tmp3 == -1)
                    return 0;
                else
                    code[1] = symtab[tmp3].dsp - (cnt + 2);
            } else
                code[1] = 0;
            code[0] = 0x7e;
            return 2;
            break;

        case 5: //nop
            code[0] = 0x90;
            return 1;
            break;
        
        case 6: //xor
            regn = findreg(pol[dis]);
            regn2 = findreg(pol[dis + 1]);
            if(regn != -1)
            {
                if(regn != -1)
                {
                    code[0] = 0x33;
                    bcode.c = 0xc0;
                    temp.c = regn;
                    bcode.b.b3 = temp.b.b0;
                    bcode.b.b4 = temp.b.b1;
                    regn = findreg(pol[dis + 1]);
                    temp.c = regn;
                    bcode.b.b0 = temp.b.b0;
                    bcode.b.b1 = temp.b.b1;
                    bcode.b.b2 = temp.b.b2;
                    code[1] = bcode.c;
                    return 2;
                } else
                {
                    if(!regn || !regn2)
                    {
                        bcode.c = 0x33;
                        dis++;
                        code[0] = bcode.c;
                        bcode.c = 0x06;
                        temp.c = regn;
                        bcode.b.b3 = temp.b.b0;
                        bcode.b.b4 = temp.b.b1;
                        bcode.b.b5 = temp.b.b2;
                        code[1] = bcode.c;
                        if(npr)
                        {
                            tmp3 = findsym(pole[dis]);
                            if(tmp3 == -1)
                                return 0;
                            else
                            {
                                tmp2.d = symtab[tmp3].dsp;
                                code[2] = tmp2.c[0];
                                code[3] = tmp2.c[1];
                            }
                            modif[pmdf] = cnt + 2;
                            pmdf++;
                            modif[pmdf] = symtab[tmp3].sgm;
                            pmdf++;
                        } else
                        {
                            code[2] = 0;
                            code[3] = 0;
                        }
                        return 4;
                    } else
                    {
                        code[0] = 0x35;
                        tmp2.d = str2num(pol[dis + 1]);
                        code[1] = tmp2.c[0];
                        code[2] = tmp2.c[1];
                        return 3;
                    }
                }
            }
            break;

        case 7: //ror
            regn = findreg(pol[dis]);
            code[0] = 0xd1;
            bcode.c = 0xc8;
            temp.c = regn;
            bcode.b.b0 = temp.b.b0;
            bcode.b.b1 = temp.b.b1;
            bcode.b.b2 = temp.b.b2;
            code[1] = bcode.c;
            return 2;
            break;
        
        case 8: //dw
            tmp2.d = str2num(pol[dis]);
            code[0] = tmp2.c[0];
            code[1] = tmp2.c[1];
            if(tmp2.d != -1)
                return 2;
            break;

        case 9: //dec
            regn = findreg(pol[dis]);
            if(regn != -1)
            {
                bcode.c = 0x48;
                temp.c = regn;
                bcode.b.b0 = temp.b.b0;
                bcode.b.b1 = temp.b.b1;
                bcode.b.b2 = temp.b.b2;
                code[0] = bcode.c;
                return 1;
            }
            break;

        case 10: //lahf
            code[0] = 0x9f;
            return 1;
            break;
        
        case 11:
            if(npr)
            {
                tmp3 = findsym(pole[dis]);
                if(tmp3 == -1)
                    return 0;
                else
                    code[1] = symtab[tmp3].dsp - (cnt + 2);
            } else
                code[1] = 0;
            
            code[0] = 0x74;
            return 2;

        case 12: //shr
            regn = findreg(pol[dis]);
            code[0] = 0xd1;
            bcode.c = 0xe8;
            temp.c = regn;
            bcode.b.b0 = temp.b.b0;
            bcode.b.b1 = temp.b.b1;
            bcode.b.b2 = temp.b.b2;
            code[1] = bcode.c;
            return 2;
            break;
        
        case 13: //or
            regn = findreg(pol[dis]);
            regn2 = findreg(pol[dis + 1]);
            if(regn != -1)
            {
                if(regn2 != -1)
                {
                    code[0] = 0x0b;
                    bcode.c = 0xc0;
                    temp.c = regn;
                    bcode.b.b3 = temp.b.b0;
                    bcode.b.b4 = temp.b.b1;
                    bcode.b.b5 = temp.b.b2;
                    regn = findreg(pol[dis + 1]);
                    temp.c= regn;
                    bcode.b.b0 = temp.b.b0;
                    bcode.b.b1 = temp.b.b1;
                    bcode.b.b2 = temp.b.b2;
                    code[1] = bcode.c;
                    return 2;
                } else
                {
                    if(!regn || !regn2)
                    {
                        bcode.c = 0x0b;
                        dis++;
                        code[0] = bcode.c;
                        bcode.c = 0x06;
                        temp.c = regn;
                        bcode.b.b3 = temp.b.b0;
                        bcode.b.b4 = temp.b.b1;
                        bcode.b.b5 = temp.b.b2;
                        code[1] = bcode.c;
                        if(npr)
                        {
                            tmp3 = findsym(pole[dis]);
                            if(tmp3 == -1)
                                return 0;
                            else
                            {
                                tmp2.d = symtab[tmp3].dsp;
                                code[2] = tmp2.c[0];
                                code[3] = tmp2.c[1];
                            }
                            modif[pmdf] = cnt + 2;
                            pmdf++;
                            modif[pmdf] = symtab[tmp3].sgm;
                            pmdf++;
                        } else
                        {
                            code[2] = 0;
                            code[3] = 0;
                        }
                        return 4;
                    } else
                    {
                        code[0] = 0x0d;
                        tmp2.d = str2num(pol[dis + 1]);
                        code[1] = tmp2.c[0];
                        code[2] = tmp2.c[1];
                        return 3;
                    }
                }
            }
        
        case 14: //int
            code[0] = 0xcd;
            code[1] = str2num(pol[dis]);
            if(code[1] != -1)
                return 2;

            break;
    }
    return 0;
}

int str2num(char str[8])
{
    int zn = 0, j = 0;
    if(str[strlen(str) - 1] == 'h')
    {
        int k;
        for(k = strlen(str) - 2; k >= 0; k++)
        {
            if(findch(str[k]) == -1)
                return -1;
            
            zn += findch(stk[k])*step(16, j);
            j++;
        }
    } else
    {
        int k;
        for(k = strlen(str) - 1; k >= 0; k--)
        {
            if(findch(str[k]) == -1)
                return -1;
            
            zn += findch(str[k])*step(10, j);
            j++;
        }
    }
    return zn;
}

int findch(char c)
{
    int i;
    for(i = 0; i <= 15; i++)
    {
        if(hextab[i] == c)
            return i;
    }
    return -1;
}

int step(int a, int b)
{
    int z = 1;
    int i;
    for(i = 0; i < b; i++)
    {
        z = z*a;
    }
    return z;
}

int offset(int a, char *str)
{
	int sum = 0;
	while(*str)
	{
		sum += *str;
		str++;
	}	
    return 256 - ((a + sum));
}