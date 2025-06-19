/*
Will do the following: 
1) Will initialize to read from the non persistent memory mapping from worker1
2) In a separate function will do the following: will get the numbers a b c r. If r=1 then will compute p as the semi-perimeter of a b c. otherwise p=0. Will sent to worker 1 via the same non persistent memory mapping.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define SHM_SIZE 4 * sizeof(float)
#define BASEDIR "/home/skullface/git/OS_2025/OS_SESSION_MODEL_2024"
#define SHM_PATH BASEDIR "/share_mem_file"

typedef struct {
    float a, b, c;
    int r;
} TripletCheck;

typedef struct {
    float a, b, c;
    float p;
} TripletProcess;


void compute_semiperimeter(){
    int shm_fd = shm_open(SHM_PATH, O_RDWR, 0666);

    void *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    while(1){
        TripletCheck data;
        memcpy(&data, shm_ptr, sizeof(TripletCheck));

        if(data.a == 0 && data.b == 0 && data.c == 0) break;

        float p = 0;
        if(data.r==1){
            p = (data.a+data.b+data.c)/2;
        }

        TripletProcess result = {data.a, data.b, data.c, p};
        memcpy(shm_ptr, &result, sizeof(TripletProcess));

        usleep(100000);
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
}

int main(){
    compute_semiperimeter();
    return 0;
}