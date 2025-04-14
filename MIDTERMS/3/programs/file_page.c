#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

int main(int argc, char * argv[])
{
    if(argc !=2)
    {
        fprintf(stderr, "USAGE: %s <file>", argv[0]);
        exit(1);
    }

    for(int i=0; argv[1][i] != '\0'; i++)
    {
        if(argv[1][i]=='+')
        {
            perror("File must not contain the character +");
            exit(1);
        }
    }

    int fd=open(argv[1], O_RDONLY);
    if(fd==-1)
    {
        perror("OPEN FILE");
        exit(2);
    }


    int nr_pages=0;
    int buffer[BUFFER_SIZE];
    off_t file_size=lseek(fd, 0, SEEK_END);
    if(file_size==-1)
    {
        perror("LSEEK");
        close(fd);
        exit(3);
    }
    else if(file_size>0)
    {
        lseek(fd, 0, SEEK_SET);
        while(read(fd, buffer, BUFFER_SIZE)>0)
        {
            nr_pages++;
        }
    }

    char byte;

    if(nr_pages==0)
    {
        byte=0;
    }
    else 
    {
        lseek(fd, nr_pages, SEEK_SET);
        read(fd, &byte, 1);
    }


    printf("%d+++%s+++%d\n", nr_pages, argv[1], byte);

    close(fd);
    return 0;
    
}