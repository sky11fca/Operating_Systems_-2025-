/*
Write a program that follows the ping pong pattenr as follows: 
-Child is responsible to output ho
-Parent is responsible to output hi

must be done with signals.
*/


#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define NR_AFISARI 100  /* Câte perechi de mesaje "hi-ho, " dorim să fie afișate. */

void handler_sigusr1(int nr_sem)
{
	fprintf(stderr,"Debug: procesul cu PID-ul %d a primit semnalul de notificare.\n", getpid());
	/*
		Notă: acesta este doar un mesaj cu rol de debugging; se poate "ignora" la execuție prin redirectarea stderr:
		UNIX> ./a.out  2> /dev/null
	*/
}

int main()
{
	int pid,i;

	if(SIG_ERR == signal(SIGUSR1,handler_sigusr1) )
	{
		perror("Eroare la signal");  exit(1);
	}
    
	if(-1 == (pid=fork()) )
	{
		perror("Eroare la fork");  exit(2);
	}

	if(pid != 0)
	{ /* Tatăl : va fi responsabil cu afișarea HI-urilor... */

		for(i = 0; i < NR_AFISARI; i++)
		{
			printf("hi-");  fflush(stdout);  /* Afișez pe ecran mesajul HI. */

			kill(pid,SIGUSR1);               /* Trimit notificare către celălalt proces. */

			pause();                         /* Aștept notificarea de la celălalt proces. */
		}
	}
	else
	{ /* Fiul : va fi responsabil cu afișarea HO-urilor... */

		for(i = 0; i < NR_AFISARI; i++)
		{
			pause();                          /* Aștept notificarea de la celălalt proces. */

			printf("ho, ");  fflush(stdout);  /* Afișez pe ecran mesajul HO. */

			kill(getppid(),SIGUSR1);          /* Trimit notificare către celălalt proces. */
		}

        printf("\nSfarsit.\n");
	}
    
	return 0;
}