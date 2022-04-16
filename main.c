#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#include "variables.h"
#include "functions.h"

char *compiler_version = "Turbo Assembler 0.1";

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
    char fname[2][10] = {"t1.dat", "t2.dat"};

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
                fprintf(stderr, "Error: wrong operand!\n");
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
                fprintf(stderr, "Error: wrong operand!\n");
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

    int of = offset(0x80 + strlen(argv[1]) + 2 + 0x0 + 0xb, argv[1]);    
    fprintf(fobj, "%c%c%c%c%s%c", 0x80, strlen(argv[1]) + 2, 0x0, 0xb, argv[1], of); 
    
    of = offset(0x88 + strlen(compiler_version) + 4 + 0x0 + 0x0 + 0x0 + 0x1c, compiler_version);
    fprintf(fobj, "%c%c%c%c%c%c%s%c", 0x88, strlen(compiler_version) + 4, 0x0, 0x0, 0x0, 0x1c, compiler_version, of);
    
    of = offset(0x88 + 0x10 + 0x0 + 0x40 + 0xe9 + 0xf9 + 0x58 + 0x34 + 0x21 + strlen(argv[1]), argv[1]);
    fprintf(fobj, "%c%c%c%c%c%c%c%c%c%c%s%c", 0x88, 0x10, 0x0, 0x40, 0xe9, 0xf9, 0x58, 0x34, 0x21, strlen(argv[1]), argv[1], of);
    
    fprintf(fobj, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0x88, 0x3, 0x0, 0x40, 0xe9, 0x4c, 0x96, 0x2, 0x0, 0x0, 0x68, 0x88, 0x3, 0x0, 0x40, 0xa1, 0x94);
    
    int y;
    for(y = 0; y <= 1; y++)
    {
        
        c = 0;
        i = 0x96; c += i; fputc(i, fobj);
        i = strlen(segtab[y].name) + 2; c += i; fputc(i, fobj);
        i = 0; c += i; fputc(i, fobj);
        i = strlen(segtab[y].name); c += i; fputc(i, fobj);
        int x;
        for(x = 0; x < strlen(segtab[y].name); x++)
        {
            i = toupper(segtab[y].name[x]);
            c += i;
            fputc(i, fobj);
        }
        
        i = 0 -c; fputc(i, fobj);
        c = 0;
        i = 0x98; c += i; fputc(i, fobj);
        i = 0x7; c += i; fputc(i, fobj);
        i = 0x0; c += i; fputc(i, fobj);
        i = 0x60; c += i; fputc(i, fobj);
        i = segtab[y].len; c += i; fputc(i, fobj);
        i = 0x0; c += i; fputc(i, fobj);
        i = y + 2; c += i; fputc(i, fobj);
        i = 0x01; c += i; fputc(i, fobj);
        i = 0x01; c += i; fputc(i, fobj);
        i = 0 - c; c += i; fputc(i, fobj);
    }
    
    fprintf(fobj,"%c%c%c%c%c%c%c", 0x88, 0x4, 0x0, 0x40, 0xa2, 0x1, 0x91);

    for(y = 0; y <= 1; y++)
    {
        c = 0;
        i = 0xa0; c += i; fputc(i, fobj);
        i = segtab[y].len + 4; c += i; fputc(i, fobj);
        i = 0; c += i; fputc(i, fobj);
        i = y + 1; c += i; fputc(i, fobj);
        i = 0; c += i; fputc(i, fobj);
        i = 0; c += i; fputc(i, fobj);
        ftmp = fopen(fname[y], "rb");
        if(!ftmp)
        {
            perror(fname[y]);
            return 5;
        }
        int x;
        for(x = 0; x < segtab[y].len; x++)
        {
            i = fgetc(ftmp);
            c += i;
            fputc(i, fobj);
        }
        fclose(ftmp);
        i = 0 - c; fputc(i, fobj);
    }
    
    modif[pmdf] = '\0';
    c = 0;
    i = 0x9c; c += i; fputc(i, fobj);
    i = 2*strlen(modif) + 1; c += i; fputc(i, fobj);
    i = 0; c += i; fputc(i, fobj);
    i = 0xc4; c += i; fputc(i, fobj);
    i = modif[0]; c += i; fputc(i, fobj);
    i = 0x54; c += i; fputc(i, fobj);
    i = modif[1]; c += i; fputc(i, fobj);
    for(y = 0; y < strlen(modif); i += 2)
    {
        i = 0xc4; c += i; fputc(i, fobj);
        i = modif[y]; c += i; fputc(i, fobj);
        i = 0x54; c += i; fputc(i, fobj);
        i = modif[y+1]; c += i; fputc(i, fobj);
    }
    i = 0 - c; fputc(i, fobj);
    fprintf(fobj, "%c%c%c%c%c%c%c%c%c%c", 0x8a, 0x7, 0x0, 0xc1, 0x0, 0x2, 0x2, 0x0, 0x0, 0xaa);
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