#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define PAUSE_TIME_IN_SECONDS 3

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "USAGE: %s, <bin_file> <instructions>", argv[0]);
        exit(1);
    }


    int fd_db=open(argv[1], O_RDWR);
    if(fd_db==-1)
    {
        perror("OPEN");
        exit(1);
    }

    FILE* fd_instr=fopen(argv[2], "r");
    if(fd_instr==NULL)
    {
        perror("OPEN2");
        close(fd_db);
        exit(1);
    }


    int key, alter;

    while(fscanf(fd_instr, "%d %d", &key, &alter))
    {
        printf("PID %d: Processing command(%d, %d)\n", getpid(), key, alter);

        int found=0;
        off_t db_size=lseek(fd_db, 0, SEEK_END);
        lseek(fd_db, 0, SEEK_SET);

        int cur_key, cur_val;

        off_t entry_pos;

        while(read(fd_db, &cur_key, sizeof(int))==sizeof(int))
        {
            if(read(fd_db, &cur_val, sizeof(int))!=sizeof(int))
            {
                break;
            }

            if(cur_key==key)
            {
                found=1;
                cur_val+=alter;
                lseek(fd_db, entry_pos+sizeof(int), SEEK_SET);
                write(fd_db, &cur_val, sizeof(int));
                printf("PID: %d: Updated key %d to %d\n", getpid(), key, cur_val);
                break;
            }

            entry_pos+=2*sizeof(int);
        }

        if(!found)
        {
            lseek(fd_db, 0, SEEK_END);
            write(fd_db, &key, sizeof(int));
            write(fd_db, &alter, sizeof(int));

            printf("PID %d: appended key %d with value %d\n", getpid(), key, alter);
        }

        sleep(PAUSE_TIME_IN_SECONDS);
    }

    close(fd_db);
    close(fd_instr);
    return 0;
}