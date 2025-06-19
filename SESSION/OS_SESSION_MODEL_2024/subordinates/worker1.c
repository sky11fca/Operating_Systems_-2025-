/*
Will do the following: 
1). Will create a child process that will run the supervisor program with the given input data file. 
2) Also in main will do the necessary operations to receive information from supervisor process via anonimous pipe and send results back with a fifo file. 
3). Also in main will make the correct setup to exchange results with worker2 using a single shared memory object: using shm_open
4) In an separated function from main, the program will read from the anonimized pipe using posix calls, each triplet of floating numbers a b c sent via the supervisor, will check the following:
    If the 3 variables are lengths of a triangle (a, b, c are greater than 0 and sum of any 2 lengths are smaller than the 3rd one). Will be sent to worker 2 via the non persistent memory mapping object in a quadruple of a b c r where integer r is 0 if given triplet is a triangle, 0 otherwise
5) In another function called by the main function, the program will read line by line a series of floatin numbers: a b c p where p is the semi-perimeter of a b c. from this non persistent memory mapping will process as it follows: 
    If p!=0, Will compute the perimeter as P=2*p and Area as A=sqrt(p(p-a)(p-b)(p-c)) (HERON FORMULA). Otherwise P=0 and A=0. And then will send via the fifo file to supervisor the 5 variables a b c P A
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>

#define SHM_SIZE 4*sizeof(float)
#define FIFO_SIZE 5*sizeof(float)

#define BASEDIR "/home/skullface/git/OS_2025/OS_SESSION_MODEL_2024"
#define SHM_PATH BASEDIR "/share_mem_file"
#define FIFO_PATH BASEDIR "/a_fifo"

typedef struct {
    float a, b, c;
    int r;
} TripletCheck;

typedef struct {
    float a, b, c;
    int p;
} TripletProcess;

void check_triangle(int in_fd){
    int shm_fd = shm_open(SHM_PATH, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);

    void* shm_ptr = mmap(NULL, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    float triplet[3];

    while(read(in_fd, triplet, 3*sizeof(float))==3*sizeof(float)){
        float a = triplet[0];
        float b = triplet[1];
        float c = triplet[2];

        int r = (a > 0 && b>0 && c>0 && a+b>c &&b+c>a && a+c>b)? 1: 0;

        TripletCheck data = {a, b, c, r};
        memcpy(shm_ptr, &data, sizeof(TripletCheck));

        usleep(1000000);
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
}

void process_result(){
    int fifo_fd = open(FIFO_PATH, O_WRONLY);
    if(fifo_fd==-1){
        perror("open");
        exit(20);
    }

    int shm_fd = shm_open(SHM_PATH, O_RDWR, 0666);
    void *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    while(1){
        TripletProcess data;

        memcpy(&data, shm_ptr, sizeof(TripletProcess));

        if(data.a == 0 && data.b==0 && data.c == 0) break;

        float P, A;

        if(data.p!=0){
            P=2*data.p;
            float s = data.p;
            A = sqrtf(s*(s-data.a)*(s-data.b)*(s-data.c));
        }
        else{
            P=A=0;
        }

        float result[5] = {data.a, data.b, data.c, P, A};

        write(fifo_fd, result, FIFO_SIZE);

        memset(shm_ptr, 0, SHM_SIZE);

        usleep(1000000);
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    close(fifo_fd);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "USAGE: %s <input_file>\n", argv[0]);
    }

    check_triangle(STDIN_FILENO);
    process_result();
}