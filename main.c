#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#include "variables.h"
#include "functions.h"
#include "gen_obj.h"

char *compiler_version = "Turbo Assembler  Version 2.0";

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Error: Too few arguments\n");
        return 1;
    }
    char cod[4];
    int length, num;

    FILE *ft;
    ft = fopen(argv[1], "r+");
    if(!ft)
    {
        perror(argv[1]);
        return 2;
    }
    
    printf("\n----------First passage----------\n");
    int c;
    int nseg = 0; //segment number
    npr = 0; //passage number
    psym = 0; //pointer to end label table
    while(!feof(ft))
    {
        fgets(stk, 80, ft);
        putchar('\n');
        puts(stk);
        stk2pool(stk, pole);
        if(!strcmp(pole[1], "segment"))
        {
            nseg++;
            cnt = 0;
            strcpy(segtab[nseg - 1].name, pole[0]);
            continue;
        }
        if(!strcmp(pole[1], "ends"))
        {
            segtab[nseg - 1].len = cnt;
            printf("Add to segtab: %s %d %d\n", segtab[nseg - 1].name, nseg, segtab[nseg - 1].len);
            continue;
        }
        if((!strcmp(pole[0], "assume")) || (!strcmp(pole[0], "\0")))
            continue;
        
        num = findop(pole[0]);
        if(num)
        {
            length = op2code(num, 0, pole, cod);
            if(num == 7 || num == 12)
                length *= atoi(pole[2]);
            if(length)
            {
                printf("Length: %d\n", length);
                cnt += length;
            }
            else
            {
                fprintf(stderr, "Error: wrong 1st operand!\n");
            }
            continue;
        }
        if((pole[0][strlen(pole[0]) - 1] == ':') || (!strcmp(pole[1], "dw")))
        {
            if(pole[0][strlen(pole[0]) - 1] == ':')
                pole[0][strlen(pole[0]) - 1] = '\0';
            
            if(findsym(pole[0]) == -1)
            {
                strcpy(symtab[psym].name, pole[0]);
                symtab[psym].sgm = nseg;
                symtab[psym].dsp = cnt;
                printf("Add to symtab: %s, %d %d\n", symtab[psym].name, symtab[psym].sgm, symtab[psym].dsp);
                psym++;
            }
            else
            {
                fprintf(stderr, "Error: label %s retry!\n", pole[0]);
            }
            if(!strcmp(pole[1], "\0"))
                continue;
            num = findop(pole[1]);
            if(num)
            {
                length = op2code(num, 1, pole, cod);
                if(length)
                {
                    printf("Length: %d\n", length);
                    cnt += length;
                } else
                {
                    fprintf(stderr, "Error: wrong 2nd operand!\n");
                }
            }
            else
            {
                fprintf(stderr, "Error: wrong mnemonic code!\n");
            }
        }
    }
    
    FILE *ftmp;
    //char fname[2][10] = {"t1.dat", "t2.dat"};

    printf("\n----------Second passage----------\n");
    pmdf = 0;
    npr++;
    nseg = 0;
    rewind(ft);
    while(!feof(ft))
    {
        fgets(stk, 80, ft);
        //putchar('\n');
        puts(stk);
        stk2pool(stk, pole);
        if(!strcmp(pole[1], "segment"))
        {
            nseg++;
            ftmp = fopen(fname[nseg - 1], "w+b");
            if(!ftmp)
            {
                perror(fname[nseg - 1]);
                return 3;
            }
            cnt = 0;
            continue;
        }
        if(!strcmp(pole[1], "ends"))
        {
            fclose(ftmp);
            continue;
        }
        if(!strcmp(pole[0], "assume") || (!strcmp(pole[0], "\0")))
            continue;

        num = findop(pole[0]);
        if(num)
        {
            length = op2code(num, 0, pole, cod);
            if(length)
            {
                printf("Code: ");
                int ror = 1;
                if(num == 7 || num == 12)
                    ror = atoi(pole[2]);
                
                int l;
                for(l = 0; l < ror; l++)
                {
                    for(i = 0; i < length; i++)
                    {
                        printf("%x", cod[i]);
                        putc(cod[i], ftmp);
                    }
                    cnt += length*ror;
                }
                putchar('\n');
            } else
            {
                fprintf(stderr, "Error: wrong operand (pole[0])!\n");
            }
            continue;
        }
        num = findop(pole[1]);
        if(num)
        {
            length = op2code(num, 1, pole, cod);
            if(length)
            {
                puts("Code: ");
                int ror = 1;
                if(num == 7 || num == 12)
                    ror = atoi(pole[3]);
                
                int l;
                for(l = 0; l < length; l++)
                {
                    int i;
                    for (i = 0; i < length; i++)
                    {
                        printf("%x", cod[i]);
                        putc(cod[i], ftmp);
                    }
                    cnt += length*ror;
                }
                putchar('\n');
            } else
            {
                fprintf(stderr, "Error: wrong operand (pole[1])!\n");
            }
        }
    }
    FILE *fobj;
    char *fnobj = "asm1.obj";
    fobj = fopen(fnobj, "w+b");
    if(!fobj)
    {
        perror(fnobj);
        return 4;
    }

    firstBlock(fobj, compiler_version, argv[1]);
    secondBlock(fobj, nseg, segtab);
    //thirdBlock(fobj);
    fourthBlock(fobj);
    
    printf("SYMTAB\n");
    printf("%10s%10s%10s\n", "Name", "Disp", "Seg");
    for(i = 0; i < psym; i++)
        printf("%10s%10d%10d\n", symtab[i].name, symtab[i].dsp, symtab[i].sgm);

    putchar('\n');
    puts("SEGTAB\n");
    printf("%10s%10s%10s\n", "Name", "N", "Length");
    for(i = 0; i < 2; i++)
        printf("%10s%10d%10d\n", segtab[i].name, i, segtab[i].len);

    putchar('\n');
    fclose(ft);
    fclose(fobj);

    return 0;

}