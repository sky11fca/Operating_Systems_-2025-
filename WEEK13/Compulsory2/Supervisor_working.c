/*
Write a program that has 1 parrent process and 2 child processes as it follows


Parent:
-Will read from file: input-data.txt up untill the EOF. And will send to the first child process the contents using anonimized pipeline
-Will wait for CHILD2 to get an response,
-Will display the result


CHILD1: 
-Will received the contents from parent process and will select only characters of base 16 numbering system, In case of upper cases it will lower case it. 
-THe contents will be shared to te second child via a fifo.  

CHILD2:
-WILL receive the contents from child process via fifo
-Will write into the file statistics.txt the character along side the number of apparitions on the buffer, will send to an anonymous pipeline to parent process the total number of distinct characters.

*/

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
#define WRITE_END 0
#define BUFFER_SIZE 1024
#define READ_END 1

typedef struct{
    char c;
    int cnt;
}HexCount;


char hex_characters[] = "aAbBcCdDeEfF";
char hex_characters_upper[] = "ABCDEF";

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

    const *buffer = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(buffer==MAP_FAILED){
        perror("mmap");
        unlink(FIFO_FILE);
        close(fd);
        exit(4);
    }

    close(fd);

    pid_t pid1, pid2;
    int p1[2], p2[2];

    if(pipe(p1) == -1 || pipe(p2) == -1){
        perror(pipe);
        unlink(FIFO_FILE);
        munmap(buffer, st.st_size);
        exit(5);
    }

    //CHILD PROCESSES
    //CHILD PROCESS 1
    if((pid1=fork())==0){
        close(p1[WRITE_END]);
        close(p2[READ_END]);
        close(p2[WRITE_END]);

        int fd_fifo = open(FIFO_FILE, O_WRONLY, 0666);
        
        char *parrent_buffer;
        read(p1[READ_END], parrent_buffer, BUFFER_SIZE);
        close(p1[READ_END]);


        char hex_buffer[BUFFER_SIZE]="";
        int new_size=0;

        for(int i=0; i<BUFFER_SIZE; i++){
            if(isdigit(parrent_buffer[i]))
            {
                hex_buffer[new_size]=parrent_buffer[i];
                new_size++;
            }
            else if(strcmp(parrent_buffer[i], hex_characters)!=0){
                if(strcmp(parrent_buffer[i], hex_characters_upper)){
                    char lower = tolower(parrent_buffer[i]);
                    hex_buffer[new_size]=lower;
                    new_size++;
                }
                else{
                    hex_buffer[new_size]=parrent_buffer[i];
                    new_size++;
                }
            }
        }

        write(fd_fifo, hex_buffer, new_size);
        close(fd_fifo);
        exit(0);
    }

    //CHILD PROCESS 2
    if((pid2==fork())==0){
        close(p1[READ_END]);
        close(p1[WRITE_END]);
        close(p2[READ_END]);
        
        HexCount h[16];

        for(int i=0; i<16; i++){
            h[i].cnt = 0;
        }

        h[0].c = '0';
        h[1].c = '1';
        h[2].c = '2';
        h[3].c = '3';
        h[4].c = '4';
        h[5].c = '5';
        h[6].c = '6';
        h[7].c = '7';
        h[8].c = '8';
        h[9].c = '9';
        h[10].c = 'a';
        h[11].c = 'b';
        h[12].c = 'c';
        h[13].c = 'd';
        h[14].c = 'e';
        h[15].c = 'f';

        int fd_fifo2 = open(FIFO_FILE, O_RDONLY);
        char c;
        while(read(fd_fifo2, &c, 1) >0){
            switch(c){
                case '0':
                    h[0].cnt++;
                    break;
                case '1':
                    h[1].cnt++;
                    break;
                case '2':
                    h[2].cnt++;
                    break;
                case '3':
                    h[3].cnt++;
                    break;
                case '4':
                    h[4].cnt++;
                    break;
                case '5':
                    h[5].cnt++;
                    break;
                case '6':
                    h[6].cnt++;
                    break;
                case '7':
                    h[7].cnt++;
                    break;
                case '8':
                    h[8].cnt++;
                    break;
                case '9':
                    h[9].cnt++;
                    break;
                case 'a':
                    h[10].cnt++;
                    break;
                case 'b':
                    h[11].cnt++;
                    break;
                case 'c':
                    h[12].cnt++;
                    break;
                case 'd':
                    h[13].cnt++;
                    break;
                case 'e':
                    h[14].cnt++;
                    break;
                case 'f':
                    h[15].cnt++;
                    break;
            }
        }

        close(fd_fifo2);

        printf("creating statistics.txt\n");
        int fd_out = open(OUTPUT_FILE, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        int result=0;

        for(int i=0; i<16; i++){
            char* line;

            snprintf(line, BUFFER_SIZE, "%c: %d\n", h[i].c, h[i].cnt);
            write(fd_out, line, BUFFER_SIZE);
            if(h[i].cnt!=0){
                result++;
            }
        }

        write(p2[WRITE_END], result, sizeof(int));
        close(p2[WRITE_END]);
        exit(0);
    }

    //Parent process

    close(p1[READ_END]);
    close(p2[WRITE_END]);

    write(p1[WRITE_END], buffer, st.st_size);
    munmap(buffer, st.st_size);
    close(p1[WRITE_END]);
    int response;

    read(p2[READ_END], &response, sizeof(int));
    close(p2[READ_END]);
    printf("File has %d distinct hexadecimal digits\n", response);

    unlink(FIFO_FILE);
}