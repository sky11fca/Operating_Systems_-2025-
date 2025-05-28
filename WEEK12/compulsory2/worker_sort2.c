#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "USAGE: %s <shm_name> <size>\n", argv[0]);
        exit(1);
    }

    const char *shm_name = argv[1];
    size_t size = atol(argv[2]);
    

    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if(shm_fd==-1){
        perror("shmopen");
        exit(2);
    }

    if(ftruncate(shm_fd, size)==-1){
        perrro("truncate");
        close(shm_fd);
        exit(3);
    }

    char *data = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(data==MAP_FAILED){
        perror("mmap");
        close(shm_fd);
        exit(4);
    }
    close(shm_fd);


    int tempfd = open("temp", O_RDWR|O_CREAT|O_TRUNC, 0666);
    if(tempfd==-1){
        perror("open");
        munmap(data, size);
        exit(5);
    }

    if(write(tempfd, data, size)!=(ssize_t)size){
        perror("Write to temp");
        close(tempfd);
        unlink("temp");
        munmap(data, size);
        exit(6);
    }
    close(tempfd);

    pid_t pid=fork();
    if(pid<0){
        perror("fork");
        unlink("temp");
        munmap(data, size);
        exit(7);
    }

    else if(pid==0){
        execlp("sort", "sort", "-n", "-t:", "-k3", "temp", NULL);
        perror("execlp");
        exit(8);
    }



    int status;
    waitpid(pid,&status, 0);
    if(!WIFEXITED(status) || WEXITSTATUS(status)!=0){
        fprintf(stderr, "sort failed");
        unlink("temp");
        munmap(data, size);
        exit(9);
    }


    tempfd = open("temp", O_RDONLY);
    if(tempfd==-1){
        perror("fork");
        unlink("temp");
        munmap(data, size);
        exit(7);
    }

    ssize_t bytes_read = read(tempfd, data, size);
    if(bytes_read!=(ssize_t)size){
        perror("fork");
        close(tempfd);
        unlink("temp");
        munmap(data, size);
        exit(7);
    }

    close(tempfd);
    unlink("temp");
    munmap(data, size);
    return 0;
    
}