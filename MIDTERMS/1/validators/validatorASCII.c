#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#define BUFFER_SIZE 1024;

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        fprintf(stderr, "USAGE: %s <file>", argv[0]);
        exit(2);
    }

    int fd=open(argv[1], O_RDONLY);
    if(fd==-1)
    {
        perror("OPEN");
        exit(3);
    }

    char buffer;
    int upper=0;
    int lower=0;

    while(read(fd, &buffer, 1)>0)
    {
        if(isupper(buffer))
        {
            upper++;
        }
        else if(islower(buffer))
        {
            lower++;
        }
    }

    printf("%s:%d:%d\n", argv[1], upper, lower);
    return 0;
}