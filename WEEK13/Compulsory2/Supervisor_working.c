

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#define INPUT_FILE "input-data.txt"
#define OUTPUT_FILE "statistics.txt"
#define FIFO_FILE "a_fifo"
#define WRITE_END 1
//#define BUFFER_SIZE 1024
#define READ_END 0

typedef struct{
    char c;
    int cnt;
}HexCount;

int is_hex_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int main()
{

    if(mkfifo(FIFO_FILE, 0666)==-1){
        perror("MKFIFO");
        exit(1);
    }

    int fd = open(INPUT_FILE, O_RDONLY);
    if(fd==-1){
        perror("open");
        unlink(FIFO_FILE);
        exit(2);
    }

    struct stat st;

    if(fstat(fd, &st)==-1){
        perror("fstat");
        unlink(FIFO_FILE);
        close(fd);
        exit(3);
    }



    pid_t pid1, pid2;
    int p1[2], p2[2];

    if(pipe(p1) == -1 || pipe(p2) == -1){
        perror("pipe");
        unlink(FIFO_FILE);
        close(fd);
        exit(5);
    }

    printf("Initializing done\n");

    //CHILD PROCESSES
    //CHILD PROCESS 1
    pid1=fork();
    if(pid1==0){
        close(p1[WRITE_END]);
        close(p2[READ_END]);
        close(p2[WRITE_END]);

        
        
        char parrent_buffer[st.st_size];
        ssize_t byte_size = read(p1[READ_END], parrent_buffer, st.st_size);

        close(p1[READ_END]);


        char hex_buffer[st.st_size];
        int new_size=0;

        for(int i=0; i<byte_size; i++){
            if(is_hex_char(hex_buffer[i])){
                hex_buffer[new_size++] = tolower(hex_buffer[i]);
            }
        }

        int fd_fifo = open(FIFO_FILE, O_WRONLY, 0666);
        
        write(fd_fifo, hex_buffer, new_size);
        close(fd_fifo);
        exit(0);
    }

    //CHILD PROCESS 2
    
    pid2=fork();
    if(pid2==0){
        close(p1[READ_END]);
        close(p1[WRITE_END]);
        close(p2[READ_END]);
        
        HexCount h[16];

        for(int i=0; i<10; i++){
            h[i].c = '0'+i;
            h[i].cnt = 0;
        }

        for(int i=10; i<16; i++){
            h[i].c = 'a'+(i-10);
            h[i].cnt = 0;
        }

        int fd_fifo2 = open(FIFO_FILE, O_RDONLY);
        char buffer[st.st_size];

        ssize_t bytes_read = read(fd_fifo2, buffer, st.st_size);
        close(fd_fifo2);

        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] >= '0' && buffer[i] <= '9') {
                h[buffer[i] - '0'].cnt++;
            } else if (buffer[i] >= 'a' && buffer[i] <= 'f') {
                h[10 + (buffer[i] - 'a')].cnt++;
            }
        }
       


        int fd_out = open(OUTPUT_FILE, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        int result=0;

        for(int i=0; i<16; i++){
            if(h[i].cnt>0){
                result++;
                char line[32];
                int len = snprintf(line, sizeof(line), "%c: %d\n", h[i].c, h[i].cnt);
                write(fd_out, line, len);
            }
        }

        close(fd_out);

        write(p2[WRITE_END], &result, sizeof(int));
        close(p2[WRITE_END]);
        exit(0);
    }

    //Parent process
    
    printf("at Parrent process\n");

    close(p1[READ_END]);
    close(p2[WRITE_END]);

    char buffer[st.st_size];
    ssize_t bytes_read = read(fd, buffer, st.st_size);

    write(p1[WRITE_END], buffer, bytes_read);
   


    close(p1[WRITE_END]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
 
    
    
    int response;

    read(p2[READ_END], &response, sizeof(response));
    close(p2[READ_END]);
    

    
    printf("File has %d distinct hexadecimal digits\n", response);
    
    unlink(FIFO_FILE);
    
   
}