#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>

void dialog_tata(char* token)
{
   FILE *file = fopen("replica_tata.txt", "r");

   if(file==NULL)
   {
    perror("OPEN");
    return;
   }


   char line[256];

   while(fgets(line, sizeof(line), file))
   {
    while(*token!=0)
    {
        usleep(1000);
    }

    printf("[P0]: %s", line);
    fflush(stdout);

    *token=1;
   }
   printf("\n");
   fclose(file);
}

void dialog_fiu(char *token)
{
    FILE *file = fopen("replica_fiu.txt", "r");

   if(file==NULL)
   {
    perror("OPEN");
    return;
   }


   char line[256];


   while(fgets(line, sizeof(line), file))
   {
    while(*token!=1)
    {
        usleep(1000);
    }

    printf("[P1]: %s", line);
    fflush(stdout);

    *token=0;
   }
   printf("\n");
   fclose(file);
}



int main()
{

    char *token = mmap(NULL, sizeof(char), PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(token==MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    *token = 0;

    pid_t pid_fiu;

    // TODO: de inițializat cu 0 fișierul flag.bin


    /* Crearea unui proces fiu. */
    if(-1 == (pid_fiu=fork()) )
    {
        perror("Eroare la fork");  return 1;
    }

    /* Ramificarea execuției în cele două procese, tată și fiu. */
    if(pid_fiu == 0)
    {   /* Zona de cod executată doar de către fiu. */

        //printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid()); usleep(200);

        dialog_fiu(token);
    }
    else
    {   /* Zona de cod executată doar de către părinte. */

        //printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid()); usleep(200);

        dialog_tata(token);
    }

    /* Zona de cod comună, executată de către ambele procese */
    //printf("\nSfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
    munmap(token, sizeof(char));
    return 0;
}
