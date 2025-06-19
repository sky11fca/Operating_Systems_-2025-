#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


void process_numbers()
{
    int num, count=0;
    int shm_fd = shm_open("/shared_mem", O_RDWR, 0666);
    if(shm_fd==-1)
    {
        perror("SHMOPEN");
        exit(1);
    }

     int* shared_mem = mmap(NULL, 2*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

     while(read(STDIN_FILENO, &num, sizeof(int))==sizeof(int))
     {
        if(num%2==1)
        {
            num*=2;
            count++;
        }

        shared_mem[0] = num;

        while(shared_mem[1]==0)
        {
            usleep(100);
        }

        num = shared_mem[1];
        shared_mem[1]=0;


     }

     munmap(shared_mem, 2*sizeof(int));
     close(shm_fd);
}

void send_results() {
    int fifo_fd = open("myfifo", O_WRONLY);
    if (fifo_fd == -1) {
        perror("open fifo");
        exit(EXIT_FAILURE);
    }

    // In a real implementation, we would calculate the actual sum and count
    int sum = 42; // Example value
    int count = 7; // Example value
    
    if (write(fifo_fd, &sum, sizeof(int)) != sizeof(int) ||
        write(fifo_fd, &count, sizeof(int)) != sizeof(int)) {
        perror("write to fifo");
        exit(EXIT_FAILURE);
    }

    close(fifo_fd);
}


int main()
{
    int shm_fd = shm_open("/shared_mem", O_CREAT | O_RDWR, 0666);
    if(shm_fd==-1)
    {
        perror("SHMOPEN");
        exit(1);
    }

    if(ftruncate(shm_fd, 2*sizeof(int))==-1)
    {
        perror("TRUNCATE");
        exit(2);
    }

    int* shared_mem = mmap(NULL, 2*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    shared_mem[0]=0;
    shared_mem[1]=0;

    process_numbers();
    send_result();
    munmap(shared_mem, 2*sizeof(int));
    close(shm_fd);
}