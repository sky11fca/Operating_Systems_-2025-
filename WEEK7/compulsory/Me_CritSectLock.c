//Same program with locks

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
    struct flock lock;


    while(1)
    {
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_CUR;
        lock.l_start = pos;
        lock.l_len = sizeof(Produce);

        fcntl(fd, F_SETLKW, &lock);

        if(read(fd, &p, sizeof(Produce)) != sizeof(Produce))
        {
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLKW, &lock);
            break;
        }


        if(p.code == code)
        {
            found=1;
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            break;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        pos+=sizeof(Produce);
    }

    if(found)
    {

        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_CUR;
        lock.l_start = pos;
        lock.l_len = sizeof(Produce);

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, pos, SEEK_SET);
        read(fd, &p, sizeof(Produce));

        if(stock<0 && p.stock < -stock)
        {
            printf("[PID %d] ERROR: Insuficient stock for product with id %d (Current: %.2f, given quant: %.2f)\n", getpid(), code, p.stock, -stock);
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            return;
        }

        p.stock += stock;
        lseek(fd, pos, SEEK_SET);
        write(fd, &p, sizeof(Produce));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("[PIS %d] Updated product %d: New Stock: %.2f\n", getpid(), code, p.stock);
    }
    else if(stock > 0)
    {
        p.code = code;
        p.stock = stock;

        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_CUR;
        lock.l_start = pos;
        lock.l_len = sizeof(Produce);
        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, 0, SEEK_END);
        write(fd, &p, sizeof(Produce));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

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