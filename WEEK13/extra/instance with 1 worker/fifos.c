#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_CHANNEL "socket"

int main(int argc, char* argv[])
{
    



    mkfifo(FIFO_CHANNEL, 0666);

    int fifo_fd=open(FIFO_CHANNEL, O_WRONLY);
    if(fifo_fd==-1){
        perror("FIFO");
        exit(2);
    }

    if(argc<2)
    {
        perror("INSUFICIENT PARAMETER");
        exit(1);
    }

    FILE *comm = fopen(argv[1], "r");
    if(comm==NULL){
        perror("FOPEN");
        exit(3);
    }

    char line[1024];

    while(fgets(line, 1024, comm))
    {
        line[strlen(line)-1]='\0';
        write(fifo_fd, line, 1024);
    }

}