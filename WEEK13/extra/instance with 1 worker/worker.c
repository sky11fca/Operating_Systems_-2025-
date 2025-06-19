#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_CHANNEL "socket"

int main()
{
    int fifo_fd=open(FIFO_CHANNEL, O_RDONLY);

    if(fifo_fd==-1)
    {
        perror("OPEN FIFO");
        exit(1);
    }

    while(1)
    {
        char line[1024];
        bzero(line, 1024);

        read(fifo_fd, line, 1024);
        if(line[0]=='\0'){
            break;
        }

        char *command[1024];

        char *token;
        int size=0;
        token=strtok(line, " ");

        while(token!=NULL)
        {
            command[size++]=token;
            token=strtok(NULL, " ");
        }

        pid_t pid=fork();
        if(pid==0)
        {
            execvp(command[0], command);
            perror("exec");
            exit(2);
        }
        else{
            int status;
            wait(&status);

            printf("EXECUTED WITH STATUS: %d\n", status);
        }


    }
    unlink(FIFO_CHANNEL);
}