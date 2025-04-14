#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>


unsigned char get_first_byte(int fd)
{
    unsigned char byte;
    int byte_read=read(fd, &byte, 1);
    if(byte_read==-1)
    {
        perror("READ1");
        exit(11);
    }

    if(lseek(fd, 0, SEEK_SET)==-1)
    {
        perror("RESET FILE POINTER");
        exit(12);
    }

    return byte;
}

unsigned char get_last_byte(int fd)
{
    unsigned char byte;
    if(lseek(fd, -1, SEEK_END)==-1)
    {
        perror("POSITION TO LAST BYTE");
        exit (20);
    }

    int byte_read=read(fd, &byte, 1);
    if(byte_read==-1)
    {
        perror("READ2");
        exit(21);
    }

    if(lseek(fd, 0, SEEK_SET)==-1)
    {
        perror("RESET FILE POINTER");
        exit(22);
    }

    return byte;
    
}

int main(int argc, char * argv[])
{
    if(argc!=2)
    {
        fprintf(stderr, "USAGE: %s <file>", argv[0]);
        exit(1);
    }

    int fd=open(argv[1], O_RDONLY);
    if(fd==-1)
    {
        perror("OPEN");
        exit (2);
    }

    unsigned char first_byte;
    unsigned char last_byte;

    off_t file_size=lseek(fd, 0, SEEK_END);

    if(file_size==-1)
    {
        perror("LSEEK");
        exit(3);
    }
    else if(file_size==0)
    {
        first_byte=last_byte=0;
    }
    else 
    {
        first_byte=get_first_byte(fd);
        last_byte=get_last_byte(fd);
    }

    printf("%s=%u\n", argv[1], last_byte+first_byte);
    
    close(fd);
    return 0;

   
}