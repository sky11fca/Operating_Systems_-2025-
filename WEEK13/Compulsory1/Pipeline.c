#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    int p1[2], p2[2], p3[2];

    if(pipe(p1) == -1 || pipe(p2) == -1 || pipe(p3) == -1){
        perror("PIPE");
        exit(1);
    }


    //CHILD 1: ps -eo user,comm,pid --no-headers

    if(fork()==0){
        close(p1[0]); //close read end
        dup2(p1[1], 0); //should redirect standard output to pipeline
        close(p1[1]);

        execlp("ps", "ps", "-eo", "user,comm,pid", "--no-headers", NULL);
        perror("Execlp");
        exit(2);
    }

    //CHILD 2: tr -s " "
    if(fork()==0){
        close(p1[1]); //close write end
        close(p2[0]); //close read end
        dup2(p1[0], 1); //should redirect standard input to pipeline
        close(p1[0]);
        dup2(p2[1], STDOUT_FILENO);
        close(p2[1]);

        execlp("tr", "tr", "-s", " ", NULL);
        perror("Execlp");
        exit(3);
    }

    //CHILD 3: cut -d" " -f1,2,3 --output-Delimiter=:
    if(fork()==0){
        close(p2[1]); //close write end
        close(p3[0]); //close read end
        dup2(p2[0], 1); //should redirect standard input to pipeline
        close(p2[0]);
        dup2(p3[1], STDOUT_FILENO);
        close(p3[1]);

        execlp("cut", "cut", "-d", " ", "-f", "1,2,3", "--output-delimiter=:", NULL);
        perror("Execlp");
        exit(4);
    }

    //Child4: sort -k2 -t:

    if(fork()==0){
        close(p3[1]); //close read end
        dup2(p3[0], 1); //should redirect standard output to pipeline
        close(p3[0]);

        execlp("sort", "sort", "-k2", "-t:", NULL);
        perror("Execlp");
        exit(2);
    }


    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    close(p3[0]);
    close(p3[1]);

    while(wait(NULL)>0);

    return 0;
}