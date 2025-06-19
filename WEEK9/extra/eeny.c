#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

pid_t next_pid;
volatile sig_atomic_t has_token=1;

void handle_token(int sig) {
    has_token = 1;
}


int main(int argc, char *argv[]) {
    
    signal(SIGUSR1, handle_token);
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <next_pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    next_pid = atoi(argv[1]);

    while (1) {
        if (has_token) {
            printf("Eeny-");
            fflush(stdout);
            has_token = 0;
            kill(next_pid, SIGUSR1);
        }
        pause(); // Wait for a signal
    }

    return EXIT_SUCCESS;
}