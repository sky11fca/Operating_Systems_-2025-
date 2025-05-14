#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main(int argc, char* argv[])
{
    printf("P(1,1): PID:%d, PPID:%d\n", getpid(), getppid());

    for(int i=0; i<4; i++)
    {
        pid_t child_pid = fork();
        switch(child_pid)
        {
            case -1:
                perror("FORK");
                exit(1);
            case 0:
                printf("P(2,%d): PID:%d, PPID:%d\n", i+1, getpid(), getppid());

                for(int j=0; j<3; j++)
                {
                    pid_t grandchild_pid = fork();
                    if(grandchild_pid == -1)
                    {
                        perror("FORK");
                        exit(1);
                    }
                    else if(grandchild_pid == 0)
                    {
                        printf("P(3,%d): PID:%d, PPID:%d\n", i*3+j+1, getpid(), getppid());
                        exit(0);
                    }
                }
                for(int j=0; j<3; j++)
                {
                    wait(NULL);
                }
                exit(0);

        }

    }

    for(int i=0; i<4; i++)
    {
        wait(NULL);
    }

    return 0;
}