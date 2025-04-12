#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>


int main(int argc, char* argv[])
{
    if(argc !=3)
    {
        perror("USAGE: ./me_ls [-l] [-A] <file>");
        exit(1);
    }

    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char path[1024];
    char *option = argv[1];
    char *path = argv[2];

    if (strcmp(option, "-l") == 0)
    {
        dir = opendir(path);
        if (dir == NULL)
        {
            perror("opendir");
            exit(1);
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                snprintf(path, sizeof(path), "%s/%s", path, entry->d_name);
                if (stat(path, &fileStat) < 0)
                {
                    perror("stat");
                    continue;
                }
                printf("%s: %ld bytes\n", entry->d_name, fileStat.st_size);
            }
        }
        closedir(dir);
    }
    else if (strcmp(option, "-A") == 0)
    {
        dir = opendir(path);
        if (dir == NULL)
        {
            perror("opendir");
            exit(1);
        }

        while ((entry = readdir(dir)) != NULL)
        {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    }
    else
    {
        fprintf(stderr, "Invalid option: %s\n", option);
        exit(1);
    }   


}