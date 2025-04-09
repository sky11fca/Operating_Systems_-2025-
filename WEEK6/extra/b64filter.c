#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

const char b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstivwxyz0123456789+/";

void b64_encoder(const char *in, int in_len, char* out)
{
    int i, j;
    char b1, b2, b3;

    for(i=0, j=0; i<in_len; i+=3)
    {
        b1 = i<in_len?in[i]: 0;
        b2 = i+1<in_len?in[i+1]: 0;
        b3 = i+2<in_len?in[i+2]: 0;

        out[j++] = b64_alphabet[(b1>>2) & 0x3F];
        out[j++] = b64_alphabet[((b1&0x03)<<4) | ((b2 >> 4) & 0x0F)];

        if(i+1<in_len)
        {
            out[j++] = b64_alphabet[((b2 & 0x0F)<<2) | ((b3 >> 6) & 0x03)];
        }
        else
        {
            out[j++] = '=';
        }

        if(i+2<in_len)
        {
            out[j++] = b64_alphabet[b3 & 0x3F];
        }
        else
        {
            out[j++] = '=';
        }
    }

    out[j]='\0';
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        perror("USAGE: CMD <INPUT> <OUTPUT>");
        exit(1);
    }


    char buffer[3];
    char b64_buffer[5];
    ssize_t byte_read;
    char resp[10];

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

    while((byte_read = read(fd_in, buffer, 3))>0)
    {
        b64_encoder(buffer, byte_read, b64_buffer);

        write(fd_out, b64_buffer, strlen(b64_buffer));
    }


    close(fd_in);
    close(fd_out);

}