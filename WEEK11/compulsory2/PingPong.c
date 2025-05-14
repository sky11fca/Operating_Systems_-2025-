#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

void dialog_tata()
{
   FILE *file = fopen("replica_tata.txt", "r");

   if(file==NULL)
   {
    perror("OPEN");
    return;
   }


   char line[256];
   int flag_fd;
   char token = 0;

   while(fgets(line, sizeof(line), file))
   {
    do{
        flag_fd = open("flag.bin", O_RDONLY);

        if(flag_fd==-1)
        {
            perror("open");
            fclose(file);
            return;
        }

        read(flag_fd, &token, 1);
        close(flag_fd);
        usleep(1000);
    }while(token!=0);

    printf("[P0] %s", line);
    fflush(stdout);


    token=1;
    flag_fd = open("flag.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(flag_fd==-1)
    {
        perror("open");
        fclose(file);
        return;
    }

    write(flag_fd, &token, 1);
    close(flag_fd);
   }

   fclose(file);
}

void dialog_fiu()
{
    FILE *file = fopen("replica_fiu.txt", "r");

   if(file==NULL)
   {
    perror("OPEN");
    return;
   }


   char line[256];
   int flag_fd;
   char token = 0;

   while(fgets(line, sizeof(line), file))
   {
    do{
        flag_fd = open("flag.bin", O_RDONLY);

        if(flag_fd==-1)
        {
            perror("open");
            fclose(file);
            return;
        }

        read(flag_fd, &token, 1);
        close(flag_fd);
        usleep(1000);
    }while(token!=1);

    printf("[P1] %s", line);
    fflush(stdout);


    token=0;
    flag_fd = open("flag.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(flag_fd==-1)
    {
        perror("open");
        fclose(file);
        return;
    }

    write(flag_fd, &token, 1);
    close(flag_fd);
   }

   fclose(file);
}

int main()
{
    pid_t pid_fiu;

    // TODO: de inițializat cu 0 fișierul flag.bin

    int fd = open("flag.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd==-1)
    {
        perror("open");
        exit(1);
    }

    char init_token=0;
    write(fd, &init_token, 1);
    close(fd);

    /* Crearea unui proces fiu. */
    if(-1 == (pid_fiu=fork()) )
    {
        perror("Eroare la fork");  return 1;
    }

    /* Ramificarea execuției în cele două procese, tată și fiu. */
    if(pid_fiu == 0)
    {   /* Zona de cod executată doar de către fiu. */

        printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid()); usleep(200);

        dialog_fiu();
    }
    else
    {   /* Zona de cod executată doar de către părinte. */

        printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid()); usleep(200);

        dialog_tata();
    }

    /* Zona de cod comună, executată de către ambele procese */
    printf("\nSfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
    unlink("flag.bin");
    return 0;
}
