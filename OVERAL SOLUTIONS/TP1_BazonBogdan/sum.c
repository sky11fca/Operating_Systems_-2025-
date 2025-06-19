#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>

#define SIZE_OF_RECORD (sizeof(char) + sizeof(unsigned int))

typedef struct {
    char c;
    unsigned int nr;
} Record;

void display_usage(char* file)
{
    fprintf(stderr, "USAGE");
    exit(1);
}

void print_contents_bin(char* filename)
{
    int fd=open(filename, O_RDONLY);
    if(fd==-1)
    {
        perror("OPEN");
        exit(10);
    }

    Record r;

    while(read(fd, &r, SIZE_OF_RECORD)>0)
    {
        printf("%c -> %u", r.c, r.nr);
    }

    printf("END OF PRINTING!");
}

void process_file(char* name, int fd_bin)
{
    struct flock lock;

    int fd=open(name, O_RDONLY);

    if(fd==-1)
    {
        perror("OPEN");
        exit(30);
    }

    char buff;
    int found=0;

    while(read(fd, &buff, 1)>0)
    {
        Record r;
        int found=0;

        while(read(fd_bin, &r, SIZE_OF_RECORD)>0)
        {
            if(r.c == buff)
            {
                found=1;
            }
        }
    }

}

void find_word_directory(char* word, int fd_bin)
{
    DIR* dir;
    struct dirent *entry;

    struct stat status;


    char path[1024];
    dir=opendir(".");

    if(dir==NULL)
    {
        perror("OPEN CUR DIR");
        exit(20);
    }

    while((entry=readdir(dir)))
    {
        snprintf(path, 1024, "%s", entry->d_name);


        if(stat(path, &status)==-1)
        {
            perror("status");
            exit(21);
        }


        if(S_ISREG(status.st_mode))
        {
            if(strcmp(entry->d_name, word) && access(entry, R_OK|W_OK))
            {
                process_file(path, fd_bin);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc<3 || argv>4)
    {
        display_usage(argv[0]);
    }
    else if(argc==3 && strcmp(argv[1], "-words")==0)
    {
        display_usage(argv[0]);
    }

    else if(argc==4 && strcmp(argv[1], "--words")==0)
    {
        print_contents_bin(argv[2]);
        return 0;
    }


    char *filename=argv[1];
    char *textfile=argv[2];

    int fd_bin=open(filename, O_RDWR);
    if (fd_bin==0)
    {
        perror("OPEN");
        exit(2);
    }

    int fd_txt=open(textfile, O_RDONLY);
    if (fd_txt==0)
    {
        perror("OPEN");
        exit(2);
    }




}