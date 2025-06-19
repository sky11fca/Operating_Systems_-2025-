#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SIZE_MEM 3*sizeof(float) + sizeof(int)

typedef struct{
    float a;
    float b;
    float c;
}TripletIn;

typedef struct{
    float a;
    float b;
    float c;
    int r;
}TripletMem;

int isTriangle(TripletIn in)
{
    return (in.a>0 && in.b>0 && in.c>0 && in.a+in.b>in.c && in.b+in.c>in.a && in.a+in.c>in.b);
}

void process_input(float *shm_ptr)
{
    TripletIn in;
    while(1)
    {
        if(read(STDIN_FILENO, &in, sizeof(in))!=sizeof(in))
        {
            break;
        }
        
        TripletMem out;

        out.a = in.a;
        out.b = in.b;
        out.c = in.c;
        out.r=0;
        
        if(isTriangle(in))
        {
            out.r=1;
        }

        shm_ptr[0] = out.a;
        shm_ptr[1] = out.b;
        shm_ptr[2] = out.b;
        shm_ptr[3] = out.r;
    }

}

int main()
{
    int shm_fd = shm_open("/shared_mem", O_CREAT|O_RDWR, 0666);

    if(shm_fd==-1)
    {
        perror("SHMOPEN");
        exit(1);
    }

    float *shm_ptr = mmap(NULL, SIZE_MEM, PROT_READ | PROT_WRITE, MAP_PRIVATE, shm_fd, 0);
    if(shm_fd==MAP_FAILED)
    {
        perror("MMAP");
        exit(3);
    }

    process_input(shm_ptr);

    munmap(shm_ptr, SIZE_MEM);
}