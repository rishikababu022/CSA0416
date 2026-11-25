#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("Parent PID: %ld\n", (long)getpid());
    fflush(stdout);

    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            printf("Child %d: PID=%ld, PPID=%ld\n",
                   i + 1, (long)getpid(), (long)getppid());
            printf("Inspect this process with: ps -p %ld -o pid,ppid,state,comm\n",
                   (long)getpid());
            fflush(stdout);
            _exit(EXIT_SUCCESS);
        }
    }

    while (wait(NULL) > 0) {
        /* Reap all child processes. */
    }
    printf("Parent: all children have terminated.\n");
    return EXIT_SUCCESS;
}
