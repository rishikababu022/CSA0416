#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    pid_t pid;
    pid=fork();
    if(pid < 0)
    {
        printf("process creation failed");
    }
    else if (pid==0)
    {
        printf("\nchild process");
        printf("\nPID = %d",getpid());
        printf("\nPPID=%d\n",getppid());
    }
    else
    {
        printf("\nparent process");
        printf("\nPID=%d",getpid());
        printf("\nPPID=%d\n",getppid());
        wait(NULL);    
    }

    return 0;
}<img width="1536" height="575" alt="Screenshot 2026-10-15 092824" src="https://github.com/user-attachments/assets/dd7329f5-9f4b-4483-9cf7-64ee4eed082b" />
