#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define LAST_EIGHT_BYTES 8

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        fprintf(stderr, "USAGE %s <file>", argv[0]);
        exit(1);
    }

    int fd=open(argv[1], O_RDONLY);

    if(fd==-1)
    {
        perror("open");
        exit(1);
    }
    
    off_t size=lseek(fd, 0, SEEK_END);

    if(size==-1)
    {
        perror("LSEEK");
        close(fd);
        exit(2);
    }

    size_t reading_bytes = size<LAST_EIGHT_BYTES ? size : LAST_EIGHT_BYTES;

    if(lseek(fd, -reading_bytes, SEEK_END)==-1)
    {
        perror("LSEEK2");
        close(fd);
        exit(2);
    }

    char buffer[9];
    ssize_t read_bytes = read(fd, buffer, reading_bytes);

    if(read_bytes==-1)
    {
        perror("READ");
        close(fd);
        exit(2);
    }

    buffer[read_bytes]='\0';

    char number[read_bytes+1];

    int index=0;

    for(int i=0; i<read_bytes; i++)
    {
        if(isdigit(buffer[i]))
        {
            number[index++]=buffer[i];
        }
    }

    number[index]='\0';

    if(index==0)
    {
        number[0]='0';
        number[1]='\0';
    }


    printf("%s:%s\n", argv[1], number);
    return 0;
}