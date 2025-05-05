#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MAX_INTERVALS 5

volatile sig_atomic_t timeout_reached = 0;
volatile sig_atomic_t intervals_passed = 0;
volatile sig_atomic_t file_closed = 0;
volatile sig_atomic_t child_done = 0;

pid_t child_pid;
int N;

void parent_alarm_handler(int sig) {
    intervals_passed++;
    if (intervals_passed < MAX_INTERVALS) {
        printf("Atentie: nu s-a introdus niciun caracter in ultimele %d secunde!\n", N);
        alarm(N);
    } else {
        timeout_reached = 1;
    }
}

void parent_sigchld_handler(int sig) {
    child_done = 1;
}

void child_sigusr2_handler(int sig) {
    printf("Fiul: am primit USR2!\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output_file> <N>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[2]);
    if (N <= 0) {
        fprintf(stderr, "N must be a positive integer\n");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();
    if (child_pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        signal(SIGUSR2, child_sigusr2_handler);
        
        // Wait for SIGUSR2
        pause();
        
        // Open and display the file
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("fopen failed in child");
            exit(EXIT_FAILURE);
        }
        
        printf("Fiul: afisare continut fisier:\n");
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }
        fclose(file);
        
        printf("Fiul: sfarsit executie!\n");
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        signal(SIGALRM, parent_alarm_handler);
        signal(SIGCHLD, parent_sigchld_handler);
        
        // Set the first alarm
        alarm(N);
        
        FILE *output_file = fopen(argv[1], "w");
        if (output_file == NULL) {
            perror("fopen failed");
            kill(child_pid, SIGKILL);
            exit(EXIT_FAILURE);
        }
        
        int c;
        while ((c = getchar()) != EOF && !timeout_reached) {
            fputc(c, output_file);
            
            // Reset the alarm if we got input
            if (!timeout_reached) {
                alarm(N);
                intervals_passed = 0;
            }
        }
        
        fclose(output_file);
        file_closed = 1;
        
        // Send SIGUSR2 to child
        kill(child_pid, SIGUSR2);
        
        // Wait for SIGCHLD
        while (!child_done) {
            pause();
        }
        
        printf("Parinte: sfarsit executie!\n");
        
        if (timeout_reached) {
            exit(1);
        } else {
            exit(0);
        }
    }
}