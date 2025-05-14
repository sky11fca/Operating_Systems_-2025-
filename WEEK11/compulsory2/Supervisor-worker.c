#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZEOF_PAIRS 2*sizeof(int)

typedef struct{
    int a;
    int b;
}Pairs;

int N=0;

char operatii[] = {'+', '-', '*', '/'};

void calcul_tata()
{
    Pairs p;
    char op;
    int result;

    int fd1 = open("operanzi.bin", O_RDONLY);
    if(fd1==-1)
    {
        perror("OPEN");
        exit(1);
    }
    int fd2 = open("operators.txt", O_RDONLY);
    if(fd2==-1)
    {
        perror("OPEN");
        exit(1);
    }

    while(read(fd1, &p, SIZEOF_PAIRS)==SIZEOF_PAIRS && read(fd2, &op, 1)==1)
    {
        switch(op)
        {
            case '+':
                result = p.a + p.b;
                break;
            case '-':
                result = p.a - p.b;
                break;
            case '*':
                result = p.a * p.b;
                break;
            case '/':
                if(p.b==0)
                {
                    printf("ERROR: Division by zero!\n");
                    exit(1);
                }
                else
                {
                    result = p.a / p.b;
                }
                break;
            default:
                printf("ERROR: Invalid operator!\n");
                exit(1);
        }

        printf("[P0] %d %c %d = %d\n", p.a, op, p.b, result);
    }
}

void generare_fiu1()
{
    int fd = open("operanzi.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd==-1)
    {
        perror("OPEN");
        exit(1);
    }

    for(int i=0; i<N; i++)
    {
        Pairs p;
        p.a=rand()%100 + 10;
        p.b=rand()%100 + 10;

        if(write(fd, &p, SIZEOF_PAIRS)!=SIZEOF_PAIRS)
        {
            perror("WRITE");
            exit(1);
        }
    }
    close(fd);
}

void generare_fiu2()
{
    int fd = open("operators.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd==-1)
    {
        perror("OPEN");
        exit(1);
    }

    for(int i=0; i<N; i++)
    {
        char op;
        op=operatii[rand()%4];
        if(write(fd, &op, 1)!=1)
        {
            perror("WRITE");
            exit(1);
        }
    }
}

int main()
{
    pid_t pid_fiu1, pid_fiu2;

    printf("Dati numarul intreg N:");
    scanf("%d", &N);

    if(N<0)
    {
        fprintf(stderr, "ERROR: N must be a positive integer. Provided number: %d\n", N);
        exit(1);
    }

    /* Crearea procesului fiu #1. */
    if(-1 == (pid_fiu1=fork()) )
    {
        perror("Eroare la fork #1");  return 1;
    }

    /* Ramificarea execuției după primul apel fork. */
    if(pid_fiu1 == 0)
    {   /* Zona de cod executată doar de către fiul #1. */
        printf("\n[P1] Procesul fiu 1, cu PID-ul: %d.\n", getpid());

        generare_fiu1();
        return 0;
    }
    else
    {   /* Zona de cod executată doar de către părinte. */
        printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());

        /* Crearea procesului fiu #2. */
        if(-1 == (pid_fiu2=fork()) )
        {
            perror("Eroare la fork #2");  return 2;
        }

        /* Ramificarea execuției după al doilea apel fork. */
        if(pid_fiu2 == 0)
        {   /* Zona de cod executată doar de către fiul #2. */
            printf("\n[P2] Procesul fiu 2, cu PID-ul: %d.\n", getpid());

            generare_fiu2();
            return 0;
        }
        else
        {   /* Zona de cod executată doar de către părinte. */

            wait(NULL);
            wait(NULL);
            // Mai întâi aștept terminarea ambilor fii, iar apoi fac calculul.

            calcul_tata();
            return 0;
        }
    }
}