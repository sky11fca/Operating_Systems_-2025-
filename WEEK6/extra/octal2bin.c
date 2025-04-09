#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void convert_char_to_octal(char number, int fd_out)
{
    switch(number)
    {
        case '0':
            write(fd_out, "000 ", 4);
            break;
        case '1':
            write(fd_out, "001 ", 4);
            break;
        case '2':
        write(fd_out, "010 ", 4);
            break;
        case '3':
        write(fd_out, "011 ", 4);
            break;
        case '4':
        write(fd_out, "100 ", 4);
            break;
        case '5':
        write(fd_out, "101 ", 4);
            break;
        case '6':
        write(fd_out, "110 ", 4);
            break;
        case '7':
        write(fd_out, "111 ", 4);
            break;
        default:
        printf("%c not a valid character", number);
    }
}

int main(int argc, char* argv[])
{


    if(argc != 3)
    {
        fprintf(stderr, "USAGE: %s <file_in> <file_out>");
        exit(1);
    }


    int fd_in=open(argv[1], O_RDONLY);

    if(fd_in==-1)
    {
        perror("OPEN");
        exit(1);
    }

    int fd_out=open(argv[2], O_WRONLY, 0644);
    if(fd_out==-1)
    {
        char confirmation;
        
        printf("WARNING: file %s doesn't exist, create the file? (y/n): ", argv[2]);
        fflush(stdout);
        scanf("%c", &confirmation);


        if(confirmation != 'y' && confirmation!='Y')
        {
            printf("GOODBYE");
            close(fd_in);
            exit(0);
        }

        fd_out=open(argv[2], O_WRONLY | O_CREAT, 0644);

        if(fd_out==-1)
        {
            perror("OPEN CREAT");
            close(fd_in);
            exit(1);
        }
    }
    else 
    {
        char confirmation;
        printf("WARNING: file %s already exist, override? (y/n): ", argv[2]);
        fflush(stdout);
        scanf("%c", &confirmation);


        if(confirmation != 'y'&& confirmation!='Y')
        {
            printf("GOODBYE");
            close(fd_in);
            exit(0);
        }

        fd_out=open(argv[2], O_WRONLY | O_TRUNC, 0644);

        if(fd_out==-1)
        {
            perror("OPEN CREAT");
            close(fd_in);
            exit(1);
        }
    }

    char buffer;

    while(read(fd_in, &buffer, 1)>0)
    {
        convert_char_to_octal(buffer, fd_out);
    }

    close(fd_in);
    close(fd_out);

    return 0;
}