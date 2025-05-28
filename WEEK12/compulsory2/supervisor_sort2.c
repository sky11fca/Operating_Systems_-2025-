#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define SHM_SIZE (10*1024*1024)

typedef struct{
    char* data;
    size_t size;
} WorkerData;

void distribute_lines(char *mapped_file, size_t file_size, WorkerData *workers, int N)
{
    int linecnt=0;
    char *p = mapped_file;
    while(p<mapped_file+file_size)
    {
        if(*p=='\n') linecnt++;
        p++;
    }

    p = mapped_file;
    int cur_worker=0;
    char * line_start = mapped_file;

    while(p<mapped_file+file_size)
    {
        if(*p == '\n' || p==mapped_file + file_size -1)
        {
            size_t line_len = p-line_len+1;

            if(workers[cur_worker].size + line_len > SHM_SIZE)
            {
                fprintf(stderr, "Wokrer %d buffer overflow\n", cur_worker);
                exit(20);
            }

            memcpy(workers[cur_worker].data+workers[cur_worker].size, line_start, line_len);
            workers[cur_worker].size += line_len;

            line_start=p+1;
            cur_worker = (cur_worker+1) %N;
        }
        p++;
    }
}

void merge_results(WorkerData *workers, int N)
{
    for(int i=0; i<N; i++)
    {
        if(workers[i].size>0)
        {
            fwrite(workers[i].data, 1, workers[i].size, stdout);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "USAGE: %s <Workers> <in_file>\n", argv[0]);
        exit(1);
    }

    int N = atoi(argv[1]);
    if(N<=0)
    {
        fprintf(stderr, "Number of worker processes must be positive integer\n");
        exit(2);
    }

    const char* input_file = argv[2];

    int fd = open(input_file, O_RDONLY);
    if(fd==-1)
    {
        perror("OPEN");
        exit(3);
    }

    struct stat st;
    if(fstat(fd, &st))
    {
        perror("FSTAT");
        close(fd);
        exit(4);
    }

    char *mapped_file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mapped_file==MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(5);
    }
    close(fd);

    WorkerData *workers = malloc(N*sizeof(WorkerData));
    if(!workers)
    {
        perror("malloc");
        munmap(mapped_file, st.st_size);
        exit(6);
    }

    for(int i=0; i<N; i++)
    {
        char shm_name[32];
        snprintf(shm_name, sizeof(shm_name), "/worker_sort_%d", i);

        int shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, 0666);
        if(shm_fd==-1)
        {
            perror("shm_open");
            munmap(mapped_file, st.st_size);
            free(workers);
            exit(7);
        }

        if(ftruncate(shm_fd, SHM_SIZE)==-1)
        {
            perror("ftruncate");
            shm_unlink(shm_name);
            close(shm_fd);
            munmap(mapped_file, st.st_size);
            free(workers);
            exit(8);
        }

        workers[i].data = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(workers[i].data==MAP_FAILED)
        {
            perror("mmap");
            shm_unlink(shm_name);
            close(shm_fd);
            munmap(mapped_file, st.st_size);
            free(workers);
            exit(9);
        }
        workers[i].size=0;
        close(shm_fd);
    }

    distribute_lines(mapped_file, st.st_size, workers, N);
    munmap(mapped_file, st.st_size);


    pid_t pids[N];
    for(int i=0; i<N; i++)
    {
        if((pids[i]=fork())==0)
        {
            char shm_name[32];
            snprintf(shm_name, sizeof(shm_name),"/worker_sort_%d", i);

            char size_str[32];
            snprintf(size_str, sizeof(size_str), "%zu", workers[i].size);

            execl("./worker_sort2", "./worer_sort2", shm_name, size_str, NULL);
            perror("execl");
            exit(10);
        }
        else if(pids[i]<0)
        {
            perror("fork");
            for(int j=0; j<N; j++)
            {
                char shm_name[32];
                snprintf(shm_name, sizeof(shm_name), "/worker_sort_%d", j);
                shm_unlink(shm_name);
                munmap(workers[j].data, SHM_SIZE);
            }
            free(workers);
            exit(11);
        }
    }

    int all_success = 1;
    for (int i = 0; i < N; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Worker %d failed\n", i);
            all_success = 0;
        }
    }

    if (all_success) {
        // Merge and output results
        merge_results(workers, N);
    } else {
        fprintf(stderr, "Sorting failed due to worker errors\n");
    }

    // Cleanup
    for (int i = 0; i < N; i++) {
        char shm_name[32];
        snprintf(shm_name, sizeof(shm_name), "/worker_sort_%d", i);
        shm_unlink(shm_name);
        munmap(workers[i].data, SHM_SIZE);
    }
    free(workers);

    return all_success ? EXIT_SUCCESS : EXIT_FAILURE;
}