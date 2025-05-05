#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>

#define MAX_REGISTERS 10

typedef struct 
{
    char c;
    unsigned int count;
} Register;


void display_words(const char* bin_path)
{
    int fd = open(bin_path, O_RDONLY);
    if(fd==-1)
    {
        if(errno == ENOENT)
        {
            printf("FILE NOT FOUND");
        }
        else 
        {
            perror("open");
        }
        return;
    }


    Register r;
    ssize_t byte_read;

    printf("FILE CONTENTS:\n");

    while((byte_read=read(fd, &r, sizeof(Register)))>0)
    {
        printf("CHAR: %c, COUNT: %u\n", r.c, r.count);
    }

    if(byte_read==-1)
    {
        perror("read");
    }

    close(fd);
}


int find_reg(int fd, char c, Register *r, off_t *pos)
{
    off_t posx=0;
    ssize_t byte_read;
    while((byte_read=pread(fd, r, sizeof(Register), pos))>0)
    {
        if(r->c == c)
        {
            *pos=posx;
            return 1;
        }
        pos+=sizeof(Register);
    }

    if(byte_read==-1)
    {
        perror("pread");
    }

    return 0;
}


void update_registry(const char *bin_path, char c, unsigned int count)
{
    int fd=open(bin_path, O_RDWR|O_CREAT, 0644);
    if(fd==-1)
    {
        perror("open");
        return;
    }

    if(flock(fd, LOCK_EX)==-1)
    {
        perror("flock");
        close(fd);
        return;
    }

    off_t file_size=lseek(fd, 0, SEEK_END);
    if(file_size==-1)
    {
        perror("lseek");
        flock(fd, LOCK_UN);
        close(fd);
        return;
    }


    Register r;
    off_t pos;
    int found = find_reg(fd, c, &r, &pos);

    if(found)
    {
        r.count+=count;
        if(pwrite(fd, &r, sizeof(Register), pos)==-1)
        {
            perror("pwrite");
        }
    }
    else if(file_size<MAX_REGISTERS*sizeof(Register))
    {
        r.c = c;
        r.count=count;
        if(pwrite(fd, &r, sizeof(Register), file_size)==-1)
        {
            perror("pwrite");
        }
    } else 
    {
        fprintf(stderr, "MAXIMUM NR OF REISTERS EXCEEDED\n");
    }

    flock(fd, LOCK_UN);
    close(fd);
}

void process_word(const char * word, const char *bin_path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if((dir=opendir("."))==NULL)
    {
        perror("opendir");
        return;
    }



    while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name, word)!=0) continue;
        if(stat(entry->d_name, &file_stat)==-1)
        {
            perror("stat");
            continue;
        }

        if(!S_ISREG(file_stat.st_mode)) continue;
        if((file_stat.st_mode & (S_IRUSR|S_IWUSR)) !=(S_IRUSR|S_IWUSR)) continue;

        int fd=open(entry->d_name, O_RDONLY);
        if(fd==-1)
        {
            perror("open");
            continue;
        }


        unsigned int digit_counts[10]={0};
        char buff[1024];
        ssize_t byte_read;

        while((byte_read=read(fd, buff, sizeof(buff)))>0)
        {
            for(ssize_t i = 0; i<byte_read; i++)
            {
                if(buff[i]>='0' && buff[i]<='9')
                {
                    digit_counts[buff[i]-'0']++;
                }
            }
        }

        if(byte_read==-1)
        {
            perror("read");
        }

        close(fd);

        for(int i=0; i<10; i++)
        {
            if(digit_counts[i]>0)
            {
                update_registry(bin_path, '0'+i, digit_counts[i]);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "USAGE: %s [--words] <bin_file> <txt_file>\n", argv[0]);
        exit(1);
    }

    const char *bin_path;
    const char *txt_path;
    int words_flag;


    if(strcmp(argv[1], "--words")==0)
    {
        words_flag=1;
        bin_path=argv[2];
        txt_path=NULL;
    }
    else 
    {
        bin_path=argv[1];
        txt_path=argv[2];
    }

    if(words_flag)
    {
        print_bin(bin_path);
        exit(0);
    }

    FILE *fd_txt=fopen(txt_path, "r");

    char word[256];

    while(fgets(word, sizeof(word), fd_txt))
    {
        size_t len = strlen(word);

        if(len>0 && word[len-1]=='\n') word[len-1]='\0';

        if(strlen(word)>0) process_word(word, bin_path);
    }

    if(ferror(fd_txt))
    {
        perror("fgets");
        fcloe(fd_txt);
        return 1;
    }

    flcose(fd_txt);
    return 0;
}