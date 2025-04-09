#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>


void process_directory(char* directory)
{
    DIR* dir;

    struct dirent * entry;
    struct stat file_stat;
    struct stat link_stat;

    char path[1024];

    dir=opendir(directory);

    if(dir==NULL)
    {
        perror("OPENDIR");
        exit(2);
    }

    while((entry=readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0)
        {
            continue;
        }
        snprintf(path, 1024, "%s/%s", directory, entry->d_name);

        if(strlen(path) >= 1023)
        {
            continue;
        }

        if(lstat(path, &file_stat)!=0)
        {
            continue;
        }


        if(S_ISLNK(file_stat.st_mode))
        {
            if(access(path, F_OK)!=0)
            {
                printf("DELETE LINK FILE: %s\n", path);
                unlink(path);
            }
        }
        else if(S_ISDIR(file_stat.st_mode))
        {
            process_directory(path);
        }

    }

    closedir(dir);
}



int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "USAGE: %s <directory>\n", argv[0]);
        exit(1);
    }


    char *directory = argv[1];

    if(access(directory, F_OK | R_OK) != 0)
    {
        perror("EXIST");
        exit(1);
    }

    struct stat path_stat;
    if(stat(directory, &path_stat) != 0)
    {
        perror("INFORMATION");
        exit(1);
    }

    if(!S_ISDIR(path_stat.st_mode))
    {
        perror("IS DIRECTORY");
        exit(1);
    }


    process_directory(directory);
}