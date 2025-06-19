#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define FIFO_FILE "socket"


void read_from_file(int fd, char * filename)
{
    FILE* file = fopen(filename, "r");
    if(file==NULL)
    {
        perror("FOPEN");
        exit(10);
    }

    int nr;

    while(fscanf(file, "%d", &nr)==1)
    {
        write(fd, &nr, sizeof(int));
    }

    fclose(file);
}

void recv_results()
{
    int fifo_fd = open(FIFO_FILE, O_RDONLY);
    
    int sum, count;

    read(fifo_fd, &sum, sizeof(int));
    read(fifo_fd, &count, sizeof(int));

    close(fifo_fd);

    int digit_sum=0;
    int n = abs(sum);

    while(n>0)
    {
        digit_sum+=n%10;
        n/=10;
    }

    printf("Sum of all numbers: %d\n", sum);
    printf("Nr of odd numbers multiplied: %d\n", count);
    printf("Sum of digits: %d\n");
}

int main(int argc, char* argv[])
{
    if (argc<2)
    {
        fprintf(stderr, "USAGE: %s <input_file>");
        exit(1);

    }

    mkfifo(FIFO_FILE, 0666);

    int p1[2];
    if(pipe(p1)==-1){
        perror("pipe!");
        exit(2);
    }

    pid_t pid1=fork;

    if(pid1==-1)
    {
        perror("fork");
        exit(3);
    }else if(pid1==0)
    {
        close(p1[1]);
        dup2(p1[0], 0);
        close(p1[0]);

        execl("./workers/worker1", "worker1", NULL);

        perror("execl");
        exit(3);

    }

    else{
        close(p1[0]);

        read_from_file(p1[1], argv[1]);
        close(p1[1]);

        recv_result();
        wait(NULL);

    }
}