#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAX_MMAP_SIZE 1024

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

    struct stat st;
    if(fstat(fd_in, &st)==-1)
    {
        perror("fstat");
        close(fd_in);
        exit(1);
    }


    size_t file_size = st.st_size;

    if(file_size==0)
    {
        printf("File empty!");
        close(fd_in);
        exit(1);
    }


    char *in_map = (char *)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd_in, 0);
    if(in_map==MAP_FAILED)
    {
        perror("mmap");
        close(fd_in);
        exit(1);
    }
    //in_map[strlen(in_map)-1] = '\0';
    int fd_out;
    if(access(argv[4], F_OK) == -1)
    {
        printf("File %s does not exist. Want to create it? (y/n): ", argv[4]);
        char response;
        scanf(" %c", &response);
        if(response != 'y' && response != 'Y')
        {
            printf("Exiting without creating the file.\n");
            munmap(in_map, file_size);
            close(fd_in);
            exit(0);
        }
        fd_out = open(argv[4], O_CREAT | O_RDWR, 0644);
        if(fd_out == -1)
        {
            perror("Error creating output file");
            munmap(in_map, file_size);
            close(fd_in);
            exit(1);
        }

        if(ftruncate(fd_out, file_size)==-1)
        {
            perror("ftruncate");
            munmap(in_map, file_size);
            close(fd_in);
            close(fd_out);
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
            munmap(in_map, file_size);
            close(fd_in);
            exit(0);
        }
        fd_out = open(argv[4], O_RDWR | O_TRUNC, 0644);
        if(fd_out == -1)
        {
            perror("Error opening output file");
            munmap(in_map, file_size);
            close(fd_in);
            exit(1);
        }

        if(ftruncate(fd_out, file_size)==-1)
        {
            perror("ftruncate");
            munmap(in_map, file_size);
            close(fd_in);
            close(fd_out);
            exit(1);
        }
    }

    char * out_map = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);

    if(out_map==MAP_FAILED)
    {
        perror("mmap");
        munmap(in_map, file_size);
        close(fd_in);
        close(fd_out);
        exit(1);
    }


    for(int i = 0; i<file_size; i++)
    {
        out_map[i] = (in_map[i]==c1) ? c2 : in_map[i];
    }

    if(msync(out_map, file_size, MS_SYNC)==-1)
    {
        perror("MSYNC");
    }

    munmap(out_map, file_size);
    munmap(in_map, file_size);
    
    
    close(fd_in);
    close(fd_out);

    return 0;

}