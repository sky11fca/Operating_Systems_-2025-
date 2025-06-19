#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>

#define SEM_KEY 0x1234

void bubble_sort_cu_semafoare(int *memorie_partajata, size_t marime);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s datafile\n", argv[0]);  exit(1);
    }

    // Deschidem fisierul
    int fd;
    if(-1 == (fd = open(argv[1], O_RDWR)))
    {
        perror("Eroare la deschiderea fisierului de date");  exit(2);
    }

    // Obtinem marimea fisierului
    struct stat sb;
    if(fstat(fd, &sb) == -1)
    {
        perror("Eroare la obtinerea informatiei despre fisier");  exit(3);
    }
    size_t marime_fisier = sb.st_size;

    // Mapam fisierul in memorie
    int *memorie_partajata = mmap(NULL, marime_fisier, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(memorie_partajata == MAP_FAILED)
    {
        perror("Eroare la maparea fisierului in memorie");  exit(4);
    }

    // Calculam numarul de elemente
    size_t numar_elemente = marime_fisier / sizeof(int);

    // Sortam memoria partajata
    bubble_sort_cu_semafoare(memorie_partajata, numar_elemente);

    // Curatam resursele
    if(munmap(memorie_partajata, marime_fisier) == -1)
    {
        perror("Eroare la demaparea memoriei partajate");  exit(5);
    }

    close(fd);

    fprintf(stderr, "Notificare: [PID: %d] Am terminat (contributia mea la) sortarea fisierului %s !\n", getpid(), argv[1]);
    return 0;
}

void bubble_sort_cu_semafoare(int *memorie_partajata, size_t marime)
{
    int modificare = 1;
    int semid;
    struct sembuf operatii[2];
    
    // Cream un set de semafoare (un semafor pentru fiecare element)
    if ((semid = semget(SEM_KEY, marime, IPC_CREAT | 0666)) == -1) {
        perror("Eroare la crearea semafoarelor");
        exit(6);
    }
    
    // Initializam semafoarele cu valoarea 1 (liber)
    unsigned short *vals = calloc(marime, sizeof(unsigned short));
    if (!vals) {
        perror("Eroare la alocarea valorilor semafoarelor");
        exit(7);
    }
    for (size_t i = 0; i < marime; i++) {
        vals[i] = 1;
    }
    if (semctl(semid, 0, SETALL, vals) == -1) {
        perror("Eroare la initializarea semafoarelor");
        free(vals);
        exit(8);
    }
    free(vals);

    /* Bucla while în care facem parcurgeri repetate ale vectorului. */
    while(modificare)
    {
        modificare = 0;

        for(size_t i = 0; i < marime - 1; i++)
        {
            // Pregatim operatiile pentru cele doua semafoare
            operatii[0].sem_num = i;
            operatii[0].sem_op = -1;  // Lock
            operatii[0].sem_flg = SEM_UNDO;
            
            operatii[1].sem_num = i+1;
            operatii[1].sem_op = -1;  // Lock
            operatii[1].sem_flg = SEM_UNDO;
            
            // Asteptam ambele semafoare
            if (semop(semid, operatii, 2) == -1) {
                perror("Eroare la asteptarea semafoarelor");
                exit(9);
            }
            
            // Verificam conditia de sortare
            if(memorie_partajata[i] > memorie_partajata[i+1])
            {
                modificare = 1;
                // Facem interschimbarea
                int temp = memorie_partajata[i];
                memorie_partajata[i] = memorie_partajata[i+1];
                memorie_partajata[i+1] = temp;
            }
            
            // Eliberam semafoarele in ordine inversa
            operatii[0].sem_num = i+1;
            operatii[0].sem_op = 1;  // Unlock
            operatii[0].sem_flg = SEM_UNDO;
            
            operatii[1].sem_num = i;
            operatii[1].sem_op = 1;  // Unlock
            operatii[1].sem_flg = SEM_UNDO;
            
            if (semop(semid, operatii, 2) == -1) {
                perror("Eroare la eliberarea semafoarelor");
                exit(10);
            }
        }
    }

    // Curatam semafoarele
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Eroare la stergerea semafoarelor");
        exit(11);
    }
}