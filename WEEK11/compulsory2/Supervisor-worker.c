#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SIZEOF_PAIRS 2*sizeof(int)

typedef struct {
    int a;
    int b;
    char op;
}Operation;

int N=0;

char operatii[] = {'+', '-', '*', '/'};

void calcul_tata(Operation *ops)
{

    for(int i=0; i<N; i++)
    {
        int result;
        switch(ops[i].op)
        {
            case '+':
                result = ops[i].a + ops[i].b;
                break;
            case '-':
                result = ops[i].a - ops[i].b;
                break;
            case '*':
                result = ops[i].a * ops[i].b;
                break;
            case '/':
                if(ops[i].b==0)
                {
                    printf("ERROR: Division by zero!\n");
                    exit(1);
                }
                else
                {
                    result = ops[i].a / ops[i].b;
                }
                break;
            default:
                printf("ERROR: Invalid operator!\n");
                exit(1);
        }

        printf("[P0] %d %c %d = %d\n", ops[i].a, ops[i].op, ops[i].b, result);
    }
}

void generare_fiu1(Operation *ops)
{
    for(int i=0; i<N; i++)
    {
        ops[i].a = rand()%100+10;
        ops[i].b = rand()%100+10;
    }
}

void generare_fiu2(Operation *ops)
{
    for(int i=0; i<N; i++)
    {
        ops[i].op = operatii[rand()%4];
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

    Operation *ops = mmap(NULL, N*sizeof(Operation), PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    if(ops==MAP_FAILED)
    {
        perror("mmap");
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

        generare_fiu1(ops);
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

            generare_fiu2(ops);
            return 0;
        }
        else
        {   /* Zona de cod executată doar de către părinte. */

            wait(NULL);
            wait(NULL);
            // Mai întâi aștept terminarea ambilor fii, iar apoi fac calculul.

            calcul_tata(ops);
            munmap(ops, N*sizeof(Operation));
            return 0;
        }
    }
}