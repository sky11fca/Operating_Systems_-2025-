#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

char *cmd[]={"cut", "--fields", "1,3,4,6", "-d:", "--output-delimiter= -- ", "/etc/passwd", NULL};


int main()
{
    
    pid_t pid =fork();

    if(pid==-1)
    {
        perror("FORK");
        exit(1);
    }
    else if(pid==0)
    {
        //execvp("cut", cmd);
        execv("/bin/cut", cmd); //Acelasi exercitiu dar cu execv in loc de execvp
        perror("EXEC");
        exit(1);
    }
    else{
        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status))
        {
            if(WEXITSTATUS(status)==0)
            {
                printf("Command succesfully executed\n");
            }
            else{
                printf("Command cannot be executed\n");
            }
        }
        else {
            printf("Error executing the child\n");
        }
    }

    return 0;
}