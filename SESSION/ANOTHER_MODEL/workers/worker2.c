#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_SIZE sizeof(int) * 2

void process_number()
{
     int shm_fd = shm_open("/shared_mem", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    int* shared_mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        if(shared_mem[0]!=0)
        {
            int num = shared_mem[0];
            shared_mem[1] = num*num;
            shared_mem[0]=0;
        }

        usleep(100);
    }

    munmap(shared_mem, SHM_SIZE);
    close(shm_fd);
}


int main()
{
    process_number();
    return 0;
}