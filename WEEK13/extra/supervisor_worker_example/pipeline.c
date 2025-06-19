/*
C program that has 2 child processes that does the folowing: 
- Parent process will read from user untill the appearance of 0 a stream of positive integers, will transmit each number to the first child process with an anonimized pipeline
- The first child will determine wether the given number from the parent is a prime number. Will send back to the parent process;
- If the result is positive, will output to the terminal with an appropriate message, otherwise, will sned this number to the second child. 
- The second child will receive the number and send back to the parent process an array of divisors of given number.
- Lastly the parent will display the number plus the array of divisors with an appropriate message.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_DIVISORS 100

int main()
{
    pid_t pid1, pid2;
    int p1[2], p2[2];

    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("PIPE");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if(pid1 < 0) {
        perror("Fork failed for child 1");
        exit(EXIT_FAILURE);
    }
    else if(pid1==0)
    {
        close(p1[1]); // Close write end of pipe 1
        close(p2[0]); // Close read end of pipe 2

        int nr;
        while(read(p1[0], &nr, sizeof(nr)) > 0) {
            int is_prime = 1; // Assume number is prime
            if (nr < 2) {
                is_prime = 0; // Numbers less than 2 are not prime
            } else {
                for (int i = 2; i * i <= nr; i++) {
                    if (nr % i == 0) {
                        is_prime = 0; // Found a divisor, not prime
                        break;
                    }
                }
            }
            write(p2[1], &is_prime, sizeof(is_prime)); // Send result back to parent
        }
        close(p1[0]);
        close(p2[1]);
        exit(EXIT_SUCCESS);

    }

    pid2 = fork();
    if(pid2<0)
    {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    else if(pid2==0)
    {
        close(p1[0]); // Close read end of pipe 1
        close(p2[1]); // Close write end of pipe 2

        int nr;
        while(read(p1[1], &nr, sizeof(nr)) > 0) {
            int divisors[MAX_DIVISORS];
            int count = 0;

            for (int i = 1; i <= nr; i++) {
                if (nr % i == 0) {
                    divisors[count++] = i; // Store divisor
                }
            }

            write(p2[0], &count, sizeof(count)); // Send count of divisors
            write(p2[0], divisors, count * sizeof(int)); // Send divisors
        }
        close(p1[1]);
        close(p2[0]);
        exit(EXIT_SUCCESS);

    }


    int nr;
    while(1)
    {
        printf("Please enter a positive integer (0 to exit): ");
        scanf("%d", &nr);
        if(nr == 0) {
            break; // Exit condition
        }

        write(p1[1], &nr, sizeof(nr)); // Send number to child 1
        int is_prime;

        read(p2[0], &is_prime, sizeof(is_prime)); // Read result from child 1
        if(is_prime)
        {
            printf("The number %d is a prime number.\n", nr);
        }
        else
        {
            printf("The number %d is not a prime number. Sending to child 2 for divisors.\n", nr);
            write(p1[1], &nr, sizeof(nr)); // Send number to child 2
            int divisors[MAX_DIVISORS];
            int count = 0;

            read(p2[0], &count, sizeof(count)); // Read count of divisors
            read(p2[0], divisors, count * sizeof(int)); // Read divisors

            printf("The number %d has the following divisors: ", nr);
            for(int i = 0; i < count; i++) {
                printf("%d ", divisors[i]);
            }
            printf("\n");
        }
    }
}