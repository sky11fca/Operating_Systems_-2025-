/*
KEEP SIMPLE AND GRADUALLY ADD COMPLEXITY


Given a binary file with pairs of type: key:value, Both integers, write a program such that given the pair of key and alteration, will modify the binary file according to the argument

REQUIREMENTS:
- if the key exists in the file, will modify acordingly
- if the key doesn't exist then the following must happen
    -if the value to alter is negative, then will error out
    -otherwise will append a new value. 

- The second part of the pair in argument must support the following operations: 
    - (+): update by adding the value or adda new entry
    - (-): update by subtraction (If the value is in negative will error out.)

*/

/*
extra excercises:
-modify the program such that will read from a file a list of commands of type (int [-/+]int)



*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


typedef struct{
    int code;
    int value;
} Pair;


void process_operation(int fd, int key, int value)
{
    Pair p;
    ssize_t bytes_read;
    int found = 0;
    off_t pos = 0;

    //Find key

    while((bytes_read=read(fd, &p, sizeof(Pair))))
    {
        if(bytes_read!=sizeof(Pair))
        {
            perror("Error while reading");
            exit(10);
        }

        if(p.code==key)
        {
            found=1;
            break;
        }

        pos+=sizeof(Pair);
    }



    if(found)
    {
        if(value<0 && p.value+value<0)
        {
            perror("Update cannot be done, Negative integer!");
            return;
        }

        p.value+=value;

        lseek(fd, pos, SEEK_SET);

        if(write(fd, &p, sizeof(Pair))!=sizeof(Pair))
        {
            perror("ERROR Updating");
            exit(11);
        }

        printf("Succesfully updated the entry of key %d, new value: %d\n", key, p.value);
    }
    else 
    {
        if(value<0)
        {
            perror("Provided value is negative!");
            return;
        }

        p.code=key;
        p.value=value;
        lseek(fd, 0, SEEK_END);

        if(write(fd, &p, sizeof(Pair))!=sizeof(Pair))
        {
            perror("ERROR ADDING");
            exit(11);
        }

        printf("Succesfully appended entryof key %d, new value: %d\n", key, p.value);
    }
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "USAGE: %s <Bin file> <file_of_commands>", argv[0]);
        exit(1);
    }


    int fd=open(argv[1], O_RDWR);

    if(fd==-1)
    {
        perror("OPEN: File doesn't exist! Please make one");
        exit(2);
    }

    FILE *instruct=fopen(argv[2], "r");
    if(instruct==NULL)
    {
        perror("Opening the instruction file");
        exit(4);
    }

    char line[1024];
    int key;
    int value;
    char operation;

    while(fgets(line, 1024, instruct))
    {
        sscanf(line, "%d %c%d", &key, &operation, &value);

        if(operation=='-')
        {
            value=-value;
        }
        else if(operation!='+')
        {
            char* msg;
            snprintf(msg, 1024, "ERROR: %c, invalid command!", &operation);

            write(STDERR_FILENO, msg, strlen(msg));
            exit(3);
        }

        process_operation(fd, key, value);
        sleep(2);
    }

    close(fd);
    fclose(instruct);


    
}