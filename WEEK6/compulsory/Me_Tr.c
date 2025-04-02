#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if(argc != 5 )
    {
        fprintf (stderr, "USAGE: %s <char> <char> <filename in> <filename out>\n", argv[0]);
        exit(1);
    }
    
    char c1 = argv[1][0];
    char c2 = argv[2][0];
    int fd_in = open(argv[3], O_RDONLY);
    if(fd_in == -1)
    {
        perror("Error opening input file");
        exit(1);
    }
    int fd_out;
    if(access(argv[4], F_OK) == -1)
    {
        printf("File %s does not exist. Want to create it? (y/n): ", argv[4]);
        char response;
        scanf(" %c", &response);
        if(response != 'y' && response != 'Y')
        {
            printf("Exiting without creating the file.\n");
            close(fd_in);
            exit(0);
        }
        fd_out = open(argv[4], O_CREAT | O_WRONLY, 0644);
        if(fd_out == -1)
        {
            perror("Error creating output file");
            close(fd_in);
            exit(1);
        }
    }
    else
    {
        printf("File %s already exists. Do you want to overwrite it? (y/n): ", argv[4]);
        char response;
        scanf(" %c", &response);
        if(response != 'y' && response != 'Y')
        {
            printf("Exiting without overwriting the file.\n");
            close(fd_in);
            exit(0);
        }
        fd_out = open(argv[4], O_WRONLY | O_TRUNC);
        if(fd_out == -1)
        {
            perror("Error opening output file");
            close(fd_in);
            exit(1);
        }
    }

    char buffer;

    while(read(fd_in, &buffer, 1) > 0)
    {
        if(buffer == c1)
        {
            buffer = c2;
        }
        write(fd_out, &buffer, 1);
    }
    close(fd_in);
    close(fd_out);

    return 0;

}