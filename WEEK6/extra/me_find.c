#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#define PATH_LEN 4096

void search_file(const char* filename, const char* path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuff;
    char abs_path[PATH_LEN];

    dir = opendir(path);
    if(dir==NULL)
    {
        perror("opendir");
        exit(1);
    }

    while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0)
        {
            continue;
        }


        snprintf(abs_path, sizeof(abs_path), "%s/%s", path, entry->d_name);

        if(lstat(abs_path, &statbuff)<0)
        {
            perror("Access path");
            exit(1);
        }

        if(S_ISDIR(statbuff.st_mode))
        {
            search_file(filename, abs_path);
        }
        else if(S_ISREG(statbuff.st_mode))
        {
            if(strcmp(entry->d_name, filename)==0)
            {
                printf("%s/%s\n", path, entry->d_name);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc !=2)
    {
        fprintf(stderr, "USAGE: %s <filename>", argv[0]);
        exit(1);
    }

    const char* filename = argv[1];
    const char* home_dir;

    if((home_dir = getenv("HOME"))==NULL)
    {
        struct passwd *pw = getpwuid(getuid());

        if(pw == NULL)
        {
            perror("Getting the password");
            exit(1);
        }

        home_dir = pw->pw_dir;
    }

    search_file(filename, home_dir);

    return 0;
}