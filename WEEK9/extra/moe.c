#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

pid_t next_pid;
volatile sig_atomic_t has_token = 0;

void handle_token(int sig) {
    has_token = 1;
}

int main(int argc, char *argv[]) {
    signal(SIGUSR1, handle_token);
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <next_pid>\n", argv[0]);
        exit(1);
    }
    next_pid = atoi(argv[1]);
    
    while (1) {
        if (has_token) {
            printf("moe\n");
            fflush(stdout);
            has_token = 0;
            kill(next_pid, SIGUSR1);
        }
        pause();
    }
    
    return 0;
}