/*
the crit sect problem: 
Given multiple porcesses, a binary text with a database to store products. in store we have pairs of form code, stocks
where
code: unique identifier:int 
stock: float

MUST BE PRESENTED BINARY

On the db we perform the following updating actions

Write the program in C that evaluates these operations of buy sell stocks in a db
at a specific interval. The given commands are stored in a txt file that have the following format

code +stock or code -stock
with + and - being add or remove stock respectively

The actions are performed if the stock doesn't go negative
If there is no product registered and operation is -stock will error out else will append the new product



It doesn't say you need to use POSIX IO operations only "YIPPIE"

*/

//this is the no lock variant (UNSAFE)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef struct{
    int code;
    float stock;
} Produce;

void process_operation(int fd, int code, float stock)
{
    Produce p;
    int found = 0;
    off_t pos = 0;


    while(read(fd, &p, sizeof(Produce)) == sizeof(Produce))
    {
        if(p.code==code)
        {
            found = 1;
            break;
        }

        pos+=sizeof(Produce);
    }

    if(found)
    {
        if(stock<0 && p.stock < -stock)
        {
            printf("[PID %d] ERROR: Insuficient stock for product with id %d (Current: %.2f, given quant: %.2f)\n", getpid(), code, p.stock, -stock);
            return;
        }

        p.stock += stock;
        lseek(fd, pos, SEEK_SET);
        write(fd, &p, sizeof(Produce));
        printf("[PIS %d] Updated product %d: New Stock: %.2f\n", getpid(), code, p.stock);
    }
    else if(stock > 0)
    {
        p.code = code;
        p.stock = stock;
        lseek(fd, 0, SEEK_END);
        write(fd, &p, sizeof(Produce));
        printf("[PID %d] Added product %d with stock %.2f\n", getpid(), code, stock);
    }
    else 
    {
        printf("[PID %d]ERROR: Product %d doesn't exist and cannot be sold\n", getpid(),code);
        exit(1);
    }
}

int main(int argc, char*argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "USAGE: %s <Database> <Command File>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDWR);
    if(fd==-1)
    {
        perror("OPEN");
        exit(1);
    }

    FILE* instr = fopen(argv[2], "r");
    if(!instr)
    {
        perror("FOPEN");
        close(fd);
        exit(1);
    }

    int code;
    char op;
    float stock;

    while(fscanf(instr, "%d %c%f", &code, &op, &stock)==3)
    {
        if(op=='-')
        {
            stock=-stock;
        }
        else if(op!='+')
        {
            fprintf(stderr, "Invalid operator %c \n", op);
            continue;
        }

        process_operation(fd, code, stock);
        sleep(5);
    }

    fclose(instr);
    close(fd);
    return 0;
}