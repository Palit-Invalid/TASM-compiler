#include<ctype.h>
#include<string.h>
#include "gen_obj.h"

int firstBlock(FILE *f, char *compiler, char *filename)
{
    char *tmp;
    int counter = 0, byte = 0;

    //Record filename
    byte = 0x80; counter += byte; fputc(byte, f);
    byte = strlen(filename) + 2; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = strlen(filename); counter += byte; fputc(byte, f);
    for(tmp = filename; *tmp; tmp++)
    {
        byte = *tmp; counter += byte; fputc(byte, f);
    }
    byte = 0 - counter; counter = 0; fputc(byte, f);

    //Record compiler name
    byte = 0x88; counter += byte; fputc(byte, f);
    byte = strlen(compiler) + 3; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    //byte = strlen(compiler); counter += byte; fputc(byte, f);
    for(tmp = compiler; *tmp; tmp++)
    {
        byte = *tmp; counter += byte; fputc(byte, f);
    }
    byte = 0 - counter; counter = 0; fputc(byte, f);

    //Additional record
    byte = 0x88; counter += byte; fputc(byte, f);
    byte = strlen(filename) + 8; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x40; counter += byte; fputc(byte, f);
    byte = 0xe9; counter += byte; fputc(byte, f);
    byte = 0x01; counter += byte; fputc(byte, f);
    byte = 0x54; counter += byte; fputc(byte, f);
    byte = 0x34; counter += byte; fputc(byte, f);
    byte = 0x21; counter += byte; fputc(byte, f);
    byte = strlen(compiler); counter += byte; fputc(byte, f);
    for(tmp = filename; *tmp; tmp++)
    {
        byte = *tmp; counter += byte; fputc(byte, f);
    }
    byte = 0 - counter; counter = 0; fputc(byte, f);

    //Constant record
    byte = 0x88; counter += byte; fputc(byte, f);
    byte = 0x3; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x40; counter += byte; fputc(byte, f);
    byte = 0xe9; counter += byte; fputc(byte, f);
    byte = 0 - counter; counter = 0; fputc(byte, f);
    
    byte = 0x96; counter += byte; fputc(byte, f);
    byte = 0x2; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0 - counter; counter = 0; fputc(byte, f);

    byte = 0x88; counter += byte; fputc(byte, f);
    byte = 0x3; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x40; counter += byte; fputc(byte, f);
    byte = 0xa1; counter += byte; fputc(byte, f);
    byte = 0 - counter; counter = 0; fputc(byte, f);
}

int secondBlock(FILE *f, int num_segments, struct segt *segtab)
{
    char *tmp;
    int counter = 0, byte = 0, y;
    for(y = 0; y < num_segments; y++)
    {
        byte = 0x96; counter += byte; fputc(byte, f);
        byte = strlen(segtab[y].name) + 2; counter += byte; fputc(byte, f);
        byte = 0; counter += byte; fputc(byte, f);
        byte = strlen(segtab[y].name); counter += byte; fputc(byte ,f);
        for(tmp = segtab[y].name; *tmp; tmp++)
        {
            byte = toupper(*tmp); counter += byte; fputc(byte, f);
        }
        
        byte = 0 - counter; counter = 0;  fputc(byte, f);

        byte = 0x98; counter += byte; fputc(byte, f);
        byte= 0x7; counter += byte; fputc(byte, f);
        byte = 0x0; counter += byte; fputc(byte, f);
        byte = 0x60; counter += byte; fputc(byte, f);
        byte = segtab[y].len; counter += byte; fputc(byte, f);
        byte = 0x0; counter += byte; fputc(byte, f);
        byte = y + 2; counter += byte; fputc(byte, f);
        byte = 0x01; counter += byte; fputc(byte, f);
        byte = 0x01; counter += byte; fputc(byte, f);
        byte = 0 - counter; counter = 0; fputc(byte, f);
    }

    fprintf(f,"%c%c%c%c%c%c%c", 0x88, 0x4, 0x0, 0x40, 0xa2, 0x1, 0x91); //record start segment

    for(y = 0; y < num_segments; y++)
    {
        byte = 0xa0; counter += byte; fputc(byte, f);
        byte = segtab[y].len + 4; counter += byte; fputc(byte, f);
        byte = 0; counter += byte; fputc(byte, f);
        byte = y + 1; counter += byte; fputc(byte ,f);
        byte = 0; counter += byte; fputc(byte, f);
        byte = 0; counter += byte; fputc(byte, f);

        FILE *ftmp = fopen(fname[y], "rb");
        if(!ftmp)
        {
            perror(fname[y]);
            return 5;
        }
        int x;
        for(x = 0; x < segtab[y].len; x++)
        {
            byte = fgetc(ftmp);
            counter += byte;
            fputc(byte, f);
        }
        fclose(ftmp);
        byte = 0 - counter; fputc(byte, f);
    }
}

int thirdBlock(FILE *f)
{
    modif[pmdf] = '\0';
    int counter = 0, byte = 0;
    byte = 0x9c; counter += byte; fputc(byte, f);
    byte = 2*strlen(modif) + 1; counter += byte; fputc(byte, f);
    byte = 0; counter += byte; fputc(byte, f);
    byte = 0xc4; counter += byte; fputc(byte, f);
    byte = modif[0]; counter += byte; fputc(byte, f);
    byte = 0x54; counter += byte; fputc(byte, f);
    byte = modif[1]; counter += byte; fputc(byte, f);

    int y;
    for(y = 0; y < strlen(modif); i += 2)
    {
        byte = 0xc4; counter += byte; fputc(byte, f);
        byte = modif[y]; counter += byte; fputc(byte, f);
        byte = 0x54; counter += byte; fputc(byte, f);
        byte = modif[y+1]; counter += byte; fputc(byte, f);
    }
    byte = 0 - counter; fputc(byte, f);
}

int fourthBlock(FILE *f)
{
    int byte = 0, counter = 0;
    byte = 0x8a; counter += byte; fputc(byte, f);
    byte = 0x7; counter += byte; fputc(byte, f);
    byte = 0; counter += byte; fputc(byte, f);
    byte = 0xc1; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);

    //start segment
    byte = 0x1; counter += byte; fputc(byte, f); 
    byte = 0x1; counter += byte; fputc(byte, f);

    //segment offset
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0x0; counter += byte; fputc(byte, f);
    byte = 0 - counter; fputc(byte, f);
    //fprintf(f, "%c%c%c%c%c%c%c%c%c%c", 0x8a, 0x7, 0x0, 0xc1, 0x0, 0x2, 0x2, 0x0, 0x0, 0xaa);
}