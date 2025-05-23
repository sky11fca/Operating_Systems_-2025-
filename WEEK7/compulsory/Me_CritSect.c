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
    ssize_t bytes_read;
    int found = 0;
    off_t pos = 0;


    while((bytes_read = read(fd, &p, sizeof(Produce))))
    {
        if(bytes_read!=sizeof(Produce))
        {
            perror("READ");
            exit(1);
        }

        if(p.code==code)
        {
            found = 1;
            break;
        }

        pos+=sizeof(Produce);
    }

    if(found)
    {
        if(stock < 0 && p.stock + stock < 0)
        {
            printf("[PID %d] ERROR: Insuficient stock for product with id %d (Current: %.2f, given quant: %.2f)\n", getpid(), code, p.stock, -stock);
            return;
        }

        p.stock += stock;
        lseek(fd, pos, SEEK_SET);
        if(write(fd, &p, sizeof(Produce))!= sizeof(Produce))
        {
            perror("ERROR WRITING");
            exit(1);
        }
        
        printf("[PIS %d] Updated product %d: New Stock: %.2f\n", getpid(), code, p.stock);
    }
    else
    {
        if(stock < 0)
        {
            printf("[OID %d] ERROR: Product %d not found, cannot be sold\n", getpid(), code);
            exit(1);
        }

        p.code=code;
        p.stock=stock;
        lseek(fd, 0, SEEK_END);
        if(write(fd, &p, sizeof(Produce))!=sizeof(Produce))
        {
            perror("ERROR ADDING PRODUCT");
            exit(1);
        }

        printf("[PID %d] Added product %d with stock %.2f\n", getpid(), code, stock);
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
    float quantity;

    while(fscanf(instr, "%d %c%f", &code, &op, &quantity)==3)
    {
        if(op=='-')
        {
            quantity=-quantity;
        }
        else if(op!='+')
        {
            fprintf(stderr, "Invalid operator %c \n", op);
            continue;
        }

        process_operation(fd, code, quantity);
        sleep(3);
    }

    fclose(instr);
    close(fd);
    return 0;
}