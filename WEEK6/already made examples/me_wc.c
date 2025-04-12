#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    int nr_b, nr_w, nr_nl;
    int fc, fw, fn;

    int last;

    int i, ch;
    char *fname = NULL;
    FILE* fd;

    nr_b=nr_w=nr_nl=0;

    last=0;
    fc=fw=fn=0;

    for(i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "-c")==0) fc=1;
        else if(strcmp(argv[i], "-w")==0) fw=1;
        else if(strcmp(argv[i], "-l")==0) fn=1;
        else if(!fname) fname=argv[1];
        else printf("This program supports wc of 1 file\n");
    }


    if(argc==1 || fname==NULL)
    {
        fprintf(stderr, "USAGE: %s [-l][-w][-c] <file>", argv[0]);
        exit(2);
    }



    if(fw+fc+fn==0)
    {
        fw=fc=fn=1;
    }


    fd=fopen(fname, "r");
    if(fd==NULL)
    {
        perror("FOPEN");
        exit(3);
    }


    while(1)
    {
        ch = fgetc(fd);
        if(feof(fd)) break;


        nr_b++;

        if(isspace(ch))
        {
            if(last==1) nr_w++;
            last=2;
        }
        else last=1;

        if(ch=='\n') nr_nl++;
    }


    if(last==1) nr_w++;

    if(fc) printf("%d bytes, ", nr_b);
    if(fw) printf("%d words, ", nr_w);
    if(fn) printf("%d lines, ", nr_nl);
    printf("-> %s\n", fname);

    fclose(fd);
}