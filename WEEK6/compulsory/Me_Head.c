#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void emulate_head(int fd, int n)
{
    char buffer;
    int cnt=0;

    while(cnt<n && read(fd, &buffer, 1)>0)
    {
        if(buffer=='\n') cnt++;
        write(STDOUT_FILENO, &buffer, 1);
    }
    printf("\n");
}

void emulate_head_char(int fd, int n)
{
    char buffer;
    int cnt=0;

    while(cnt<n && read(fd, &buffer, 1)>0)
    {
        cnt++;
        write(STDOUT_FILENO, &buffer, 1);
    }
    printf("\n");
}



int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        char* option = argv[1];
        int n = atoi(argv[2]);
        int fd = open(argv[3], O_RDONLY, 0644);

        if(fd==-1)
        {
            perror("open");
            exit(1);
        }

        if(strncmp(option, "-n", 2)==0)
        {
            emulate_head(fd, n);
        }
        else if(strncmp(option, "-c", 2)==0)
        {
            emulate_head_char(fd, n);
        }
        else
        {
            printf("UNKNOWN PARAMETER: %s", option);
            exit(1);
        }
        close(fd);
    }
    else if(argc == 2)
    {
        int fd = open(argv[1], O_RDONLY, 0644);
        if(fd==-1)
        {
            perror("open");
            exit(1);
        }
        
        emulate_head(fd, 10);
        close(fd);
    }
    else
    {
        fprintf(stderr, "USAGE: %s <option> <number> <filename>\n", argv[0]);
    }



    return 0;
}