#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct
{
    float a;
    float b;
    float c;
}TripletIn;

typedef struct{
    float a;
    float b;
    float c;
    float P;
    float S;
}TripletOut;

void process_input(int fd)
{
    FILE *file = fopen("input_data.txt", "r");
    if(file==NULL)
    {
        perror("FOPEN");
        exit(5);
    }

    char line[1024];

    while(fgets(line, sizeof(line), file))
    {
        TripletIn in;

        scanf(line, "%f %f %f", &in.a, &in.b, &in.c);

        write(fd, &in, 3*sizeof(float));
    }

    fclose(file);
}

void process_output()
{
    int fifo_fd = open("a_fifo", O_RDONLY);
    if(fifo_fd==-1)
    {
        perror("open fifo");
        exit(20);
    }

    while(1)
    {
        TripletOut out;

        if(read(fifo_fd, &out, 5*sizeof(float))!= 5*sizeof(float))
        {
            break;
        }

        if(out.P!=0)
        {
            printf("The triplet %f, %f, %f is a triangle with perimeter P=%f and area A=%f\n", out.a, out.b, out.c, out.P, out.S);
        }
        else{
            printf("The triplet %f, %f, %f is not a valid triangle\n");
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "USAGE: %s, <input_file>", argv[0]);
        exit(1);
    }

    int pipe1[2];
    if(pipe(pipe1)==-1)
    {
        perror("PIPE");
        exit(2);
    }

    pid_t pid1 = fork();

    if(pid1==-1)
    {
        perror("FORK");
        exit(3);
    }
    else if(pid1==0)
    {
        close(pipe1[1]);
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        execl("./worker1", "worker1");

        perror("exec");
        exit(4);
    }
    else
    {
        close(pipe1[0]);

        process_input(pipe1[1]);

        close(pipe1[1]);

        process_output();

        wait(NULL);
        
    }

    return 0;
}
