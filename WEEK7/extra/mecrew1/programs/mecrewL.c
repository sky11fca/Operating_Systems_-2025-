#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define SIZE_OF_RECORD (2*sizeof(int)+2*sizeof(bool)+50)

typedef struct{
    int id_stud;
    char name[50];
    int year;
    bool state;
    bool budget;
} Student;

srand();

int find_next_id(int fd)
{
    Student s;

    off_t filesize=lseek(fd, 0 ,SEEK_END);
    int record_cnt=filesize/SIZE_OF_RECORD;

    if(record_cnt==0) return -1;
}

void add_entry(int fd, char *name, int year, bool state, bool budget)
{
    int id=(rand()%100) + 1; //id students from 1 to 100

    while(find_by_id(fd, id))
    {
        id=(rand()%100) + 1;
    }

    Student s={
        .id_stud = id,
        .year=year,
        .state=state,
        .budget=budget
    };

    strncpy(s.name, name, 50);

    s.name[49]='\0';

    lseek(fd, 0, SEEK_END);
    write(fd, &s, SIZE_OF_RECORD);

    printf("[PID %d]: added student: ID %d, NAME %s, YEAR %d STATE %d BUDGET %d\n", getpid(), s.id_stud, s.name, s.year, s.state, s.budget);
    //implement something to generate new random ids
    //check if the id exists find_by_id function
    //use lseek to go to the eof
    //create an student entry with newly generated id and given info
}

void update_entry(int fd, int id, int year, bool state, bool budget)
{
    //iterate through entire DB to check if id exists;
    //move LSEEK from begining of the file to the specified entry position
    //ovewrite the entry

    Student s;
    int found;
    int pos=0;
    while(read(fd, &s, SIZE_OF_RECORD)>0)
    {
        if(s.id_stud=id)
        {
            found=1;
            break;
        }
        pos+=SIZE_OF_RECORD;
    }

    if(found)
    {
        lseek(fd, pos, SEEK_CUR);
        s.year=year;
        s.state=state;
        s.budget=budget;

        write(fd, &s, SIZE_OF_RECORD);
        printf("DONE");
    }
    else {
        return;
    }
}

int find_by_id(int fd, int id)
{
    //iterate through entire DB
    //return 1 if exists
    //0 otherwise

    Student s;

    while(read(fd, &s, SIZE_OF_RECORD)>0)
    {
        if(s.id_stud=id)
        {
            printf("[PID %d], Found student with ID: %d, NAME: %s, YEAR: %d, STATE: %d, BUDGET: %d\n", getpid(), s.id_stud, s.name, s.year, s.state, s.budget);
            return 1;
        }
    }

    printf("[PID %d] NO SUCH STUDENT WITH ID: %d", getpid(), id);
    return 0;
}

int find_by_name(int fd, char* name)
{
    //iterate through entire DB to check a student with this name exists
    //returns 1 if so
    //else return 0
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "USAGE: %s <database> <instruction>", argv[0]);
        exit(1);
    }

    int fd_db=open(argv[1], O_RDWR);
    if(fd_db==-1)
    {
        perror("OPEN");
        exit(1);
    }

    FILE* fd_i=fopen(argv[2], "r");
    if(fd_i==NULL)
    {
        perror("FOPEN");
        close(fd_db);
        exit(1);
    }

    char line[1024];

    while(fgets(line, 1024, fd_i))
    {
        if(strnchr(line, "add", 3)==0)
        {
            char *command_name;
            char *name;
            int year;
            bool state;
            bool budget;

            sscanf(line, "%s %49s %d %d %d", command_name, name, &year, &state, &budget);

            add_entry(fd_db, name, year, state, budget);
        }
        else if(strnchr(line, "update", 6)==0)
        {
            char* command_name;
            int id_stud;
            int year;
            bool state;
            bool budget;

            sscanf(line, "%s %d %d %d %d", command_name, &id_stud, &year, &state, &budget);

            update_entry(fd_db, id_stud, year, state, budget);
        }
        else if(strnchr(line, "find-by-id", 10)==0)
        {
            char* command_name;
            int id_stud;

            sscanf(line, "%s %d", command_name, &id_stud);

            if(find_by_id(fd_db, id_stud)==0)
            {
                perror("FIND BY ID");
            }

        }
        else if(strnchr(line, "find-by-name", 12)==0)
        {
            char* command_name;
            char* name;

            sscanf(line, "%s %49s", command_name, name);

            if(find_by_name(fd_db, name)==0)
            {
                perror("FIND BY NAME");
            }

        }
        else printf("Invalid command!");

        sleep(5);
    }



    close(fd_db);
    fclose(fd_i);
    return 0;
}