/*
  Program: CS-synced_v1.c

  Functionality: solves the critical section problem -- synchronization is achieved using a named semaphore.
  This program is supposed to be run as a SPMD job (i.e., multiple instances of it running in parallel).

  Remark: the solution based on a binary semaphore ensures the mutual exclusion property 
  (possibly also the progress property, but not the bounded waiting property).
*/

#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define handle_error(msg,cod) \
    { perror(msg); exit(cod); }


void critical_section(int n)
{
    fprintf(stderr,"[PID: %d] In the Critical Section for the %dth time: starting to execute it...\n", getpid(), n);

    /* Doing some critical work, e.g. accessing a shared resource. */
    sleep(random() % 4); // A random pause of up to 3 seconds to simulate the duration of the calculations performed here.

    fprintf(stderr,"[PID: %d] In the Critical Section for the %dth time: finished to execute it.\n", getpid(), n);
}

void remainder_section(int n)
{
    fprintf(stderr,"[PID: %d]\tOut of CS: starting to execute the remainder section for the %dth time...\n", getpid(), n);

    /* Doing some NON critical work, i.e. NOT accessing any shared resource. */
    sleep(random() % 6); // A random pause of up to 5 seconds to simulate the duration of the calculations performed here.

    fprintf(stderr,"[PID: %d]\tOut of CS: finished to execute the remainder section for the %dth time.\n", getpid(), n);
}



int main(int argc, char *argv[])
{
    int nr_iterations;
    const char *sem_name = "/named_semaphore_obj1";
    sem_t *my_semaphore;

    /* Validate the required arguments in the command line. */
    if(argc < 2) {
        fprintf(stderr,"Usage: %s <number-of-iterations>\n", argv[0]); return 1;
    }
    if(1 != sscanf(argv[1],"%d", &nr_iterations)) {
        fprintf(stderr,"Invalid argument: %s\n", argv[1]); return 2;
    }

    /* Initialize the random number generator. */
    srandom( getpid() );

    /* Create or open the named semaphore. */
    my_semaphore = sem_open(sem_name, O_RDWR | O_CREAT, 0600, 1);
    // The initial value of the semaphore is 1 (we are using a binary semaphore to ensure mutual exclusion).
    if(SEM_FAILED == my_semaphore) handle_error("Error at sem_open",3);


    /* The main loop of the program. */
    for(int i=1; i<=nr_iterations; i++)
    {
        /* Entry section:  wait(semaphore); */
        if(-1 == sem_wait(my_semaphore)) handle_error("Error at sem_wait",4); 

        /* The critical section. */
        critical_section(i);

        /* Exit section:  signal(semaphore); */
        if(-1 == sem_post(my_semaphore)) handle_error("Error at sem_post",5);
		
        /* The remainder (non-critical) section. */ 
        remainder_section(i);
    }

    /* Close the named semaphore. */
    if(-1 == sem_close(my_semaphore)) handle_error("Error at sem_close",6);


    /* Delete the name of the named semaphore object;
       the object itself will be destroyed after all the instances of the SPMD job will close the object. */
    // if(-1 == sem_unlink(my_semaphore)) handle_error("Error at sem_unlink",7);
    /* The sem_unlink should be called only once (e.g., by the last running instance of the SPMD job), otherwise
       all the calls but the first one will return the error "Error at sem_unlink: No such file or directory"
       But this only once calling is not yet implemented here...
       Instead, we can delete the object from commandline, after the SPMD job terminates (or before starting the SPMD job). */

    return 0;
}
