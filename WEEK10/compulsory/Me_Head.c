#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

void emulate_head(const char* file_map, off_t size, int n)
{
    int cnt=0;
    size_t i=0;
    while(i<size && cnt<n)
    {
        if(file_map[i]=='\n') cnt++;
        write(STDOUT_FILENO, &file_map[i], 1);
        i++;
    }
    printf("\n");
}

void emulate_head_char(const char* file_map, off_t size, int n)
{
    int cnt=0;
    size_t i=0;

    while(i<size && cnt<n)
    {
        cnt++;
        write(STDOUT_FILENO, &file_map[i], 1);
        i++;
    }
    printf("\n");
}

int compute_nr_of_lines(const char* file_map, off_t size)
{
    int lines = 1;
    for(size_t i=0; i<size; i++)
    {
        if(file_map[i]=='\n') lines++;
    }
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

        struct stat st;
        if(fstat(fd, &st)==-1)
        {
            perror("fstat");
            exit(1);
        }

        size_t file_size = st.st_size;
        if(file_size==0)
        {
            printf("Cam bate vantu' prin fisierul asta, n-am sa te mint!\n");
            close(fd);
            exit(1);
        }


        char * file_map = (char*)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if(file_map==MAP_FAILED)
        {
            perror("MMAP");
            close(fd);
            exit(1);
        }

        if(strncmp(option, "-n", 2)==0)
        {
            if(numbers[0] == '-')
            {
                maxline = compute_nr_of_lines(file_map, file_size) - atoi(numbers+1);
            }
            else maxline = atoi(numbers);
           
            emulate_head(file_map, file_size, maxline);
        }
        else if(strncmp(option, "-c", 2)==0)
        {
            if(numbers[0] =='-')
            {
                maxsize = file_size - atoi(numbers+1);
            }
            else maxsize = atoi(numbers);
        
            emulate_head_char(file_map, file_size, maxsize);
        }
        else
        {
            printf("UNKNOWN PARAMETER: %s", option);
            munmap(file_map, file_size);
            close(fd);
            exit(1);
        }
        munmap(file_map, file_size);
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

        struct stat st;
        if(fstat(fd, &st)==-1)
        {
            perror("fstat");
            exit(1);
        }

        size_t file_size = st.st_size;
        if(file_size==0)
        {
            printf("Cam bate vantu' prin fisierul asta, n-am sa te mint!\n");
            close(fd);
            exit(1);
        }

        char * file_map = (char*)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if(file_map==MAP_FAILED)
        {
            perror("MMAP");
            close(fd);
            exit(1);
        }
        
        emulate_head(file_map, file_size, 10);
        munmap(file_map, file_size);
        close(fd);
    }
    else
    {
        fprintf(stderr, "USAGE: %s <option> <number> <filename>\n", argv[0]);
    }



    return 0;
}