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

int compute_file_size(int fd)
{
    int size = 0;
    char buffer;

    while(read(fd, &buffer, 1) > 0)
    {
        size++;
    }
    lseek(fd, 0, SEEK_SET);
    return size;
}

int compute_nr_of_lines(int fd)
{
    int lines = 1;
    char buffer;

    while(read(fd, &buffer, 1) > 0)
    {
        if(buffer == '\n') lines++;
    }
    lseek(fd, 0, SEEK_SET);
    return lines;
}



int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        int maxsize = 0;
        int maxline = 0;
        char* option = argv[1];
        char* numbers = argv[2];
        int fd = open(argv[3], O_RDONLY, 0644);

        if(fd==-1)
        {
            perror("open");
            exit(1);
        }

        if(strncmp(option, "-n", 2)==0)
        {
            if(numbers[0] == '-')
            {
                maxline = compute_nr_of_lines(fd) - atoi(numbers+1);
            }
            else maxline = atoi(numbers);
           
            emulate_head(fd, maxline);
        }
        else if(strncmp(option, "-c", 2)==0)
        {
            if(numbers[0] =='-')
            {
                maxsize = compute_file_size(fd) - atoi(numbers+1);
            }
            else maxsize = atoi(numbers);
        
            emulate_head_char(fd, maxsize);
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