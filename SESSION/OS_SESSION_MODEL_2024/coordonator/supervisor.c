/*
Will do the following: 
1). Will pass as an argument the path to the text file called input_data.txt, the file consists of a triplet of of floating a b c. First will check if there is a given argument. 
2) Then will do the necessary checks to send information to the worker1 process via 1 to 1 anonymous pipe. and get the result from worker1 process from a fifo file (again one to one communication).
3) In a separete function, the program will read line by line each triplet of floating numbers and will convert them into their binary form. And then will send to the worker1 process via anonimized pipe using only POSIX calls. 
4) In another separate function will get a quintuple of 5 floating numbers representing a b c P A where P is the perimeter and A is the area, and will send the information to the parrent process via fifo, if P!=0 will display a b c as well as P and A, and if P=0 will display an appropriate message
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 3*sizeof(float)
#define RESP_SIZE 5*sizeof(float)
#define BASEDIR "/home/skullface/git/OS_2025/OS_SESSION_MODEL_2024"
#define FIFO_PATH BASEDIR "/a_fifo"

void send_triplets(int fd, const char* filename)
{
    FILE *in_fd = fopen(filename, "r");
    if(in_fd==NULL){
        perror("FOPEN");
        exit(10);
    }

    char line[100];
    while(fgets(line, sizeof(line), in_fd)){
        float a, b, c;
        if(sscanf(line, "%f %f %f", &a, &b, &c)==3){
            float triplet[3] = {a, b, c};
            write(fd, triplet, BUFF_SIZE);
        }
    }

    fclose(in_fd);
}

void recv_results(){
    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    if(fifo_fd==-1){
        perror("OPEN");
        exit(20);
    }

    int valid = 0, invalid=0;
    float result[5];

    while(read(fifo_fd, result, RESP_SIZE)==RESP_SIZE){
        float a = result[0];
        float b = result[1];
        float c = result[2];
        float P = result[3];
        float A = result[4];

        if(P!=0){
            printf("Triplet %f, %f, %f with perimeter %.f and area %.f\n", a, b, c, P, A);
            valid++;
        }
        else{
            printf("Triplet: %f, %f, %f is not a valid triangle\n", a, b, c);
            invalid++;
        }
    }

    printf("Statistics: %d valid, %d invalid", valid, invalid);
    close(fifo_fd);
}


int main(int argc, char* argv[])
{
    if(argc != 2){
        fprintf(stderr, "USAGE: %s <input_file>\n", argv[0]);
        exit(1);
    }

    int p[2];
    if(pipe(p)==-1){
        perror("PIPE");
        exit(2);
    }

    pid_t pid = fork();
    
    if(pid==0){
        //WORKER 1

        close(p[1]);
        dup2(p[0], STDIN_FILENO);
        close(p[0]);

        execl(BASEDIR "/subordinates/worker1", "worker1", argv[1], NULL);
        perror("EXECL WORKER1");
        exit(3);
    }
    else{
        close(p[0]);
        send_triplets(p[1], argv[1]);
        close(p[1]);

        recv_results();
        wait(NULL);

    }
    return 0;
}