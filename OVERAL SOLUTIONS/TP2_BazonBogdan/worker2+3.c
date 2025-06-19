#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SIZE_MEM 3*sizeof(float) + sizeof(int)
#define SIZE_MEM2 4*sizeof(float)

typedef struct{
    float a;
    float b;
    float c;
    int r;
}TripletMem;

typedef struct{
    float a;
    float b;
    float c;
    float p;
}TripletAnonMem;

typedef struct{
    float a;
    float b;
    float c;
    float P;
    float S;
}TripletOut;


void process_worker2(float *mem_anon)
{
    int shm_fd = shm_open("/shared_mem", O_RDONLY, 0666);
    if(shm_fd==-1)
    {
        perror("SHM OPEN");
        exit(10);
    }

    TripletMem in;

    float *shm_ptr=mmap(NULL, SIZE_MEM, PROT_READ, MAP_SHARED, shm_fd, 0);
    if(shm_ptr==MAP_FAILED)
    {
        perror("MMAP");
        exit(11);
    }

    while(1)
    {
        shm_ptr[0]=in.a;
        shm_ptr[1]=in.b;
        shm_ptr[2]=in.c;
        shm_ptr[3]=in.r;

        if(in.a==0 && in.b==0 && in.c==0)
        {
            break;
        }

        TripletAnonMem out1;
        out1.a=in.a;
        out1.b=in.b;
        out1.c=in.c;
        out1.p=0;


        if(in.r==1)
        {
            out1.p=(in.a+in.b+in.c)/2;
        }

        mem_anon[0]=out1.a;
        mem_anon[1]=out1.b;
        mem_anon[2]=out1.c;
        mem_anon[3]=out1.p;


    }
    munmap(shm_ptr, SIZE_MEM);

}

void process_worker3(float* mem_anon)
{
    mkfifo("a_fifo", 0666);

    int fifo = open("a_fifo", O_RDWR, 0666);
    if(fifo==-1)
    {
        perror("open fifo");
        exit(20);
    }

    while(1)
    {
        TripletAnonMem in;
        TripletOut out2;
        in.a = mem_anon[0];
        in.b = mem_anon[1];
        in.c = mem_anon[2];
        in.p = mem_anon[3];

        if(in.a==0 && in.b==0 && in.c==0)
        {
            break;
        }

        out2.a = in.a;
        out2.b = in.b;
        out2.c = in.c;
        out2.P = 0;
        out2.S = 0;

        if(in.p!=0)
        {
            out2.P = 2*in.p;
            out2.S = sqrtf(in.p * (in.p-in.a) * (in.p-in.b) * (in.p-in.c));
        }

        if(write(fifo, &out2, 5*sizeof(float))!=5*sizeof(float))
        {
            perror("write");
            exit(21);
        }

        mem_anon[0]==0;
        mem_anon[1]==0;
        mem_anon[2]==0;
        mem_anon[3]==0;

    }
}

int main()
{
    float *mem_anon = mmap(NULL, SIZE_MEM2, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if(mem_anon==MAP_FAILED)
    {
        perror("mmap");
        exit(2);
    }
    //WHAT IS FOR THE ANON MAPPING?


    pid_t pid = fork();

    if(pid==-1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid==0)
    {
        process_worker2(mem_anon);    

        exit(0);
    }
    else
    {
        process_worker3(mem_anon);
    }

    return 0;

    munmap(mem_anon, SIZE_MEM2);
}