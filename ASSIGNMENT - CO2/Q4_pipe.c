#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        close(fd[1]);
        int value, count = 0, sum = 0, max = 0;
        ssize_t bytes;

        while ((bytes = read(fd[0], &value, sizeof value)) > 0) {
            if (bytes != (ssize_t)sizeof value) {
                fprintf(stderr, "Incomplete integer received\n");
                close(fd[0]);
                _exit(EXIT_FAILURE);
            }
            if (count == 0 || value > max) max = value;
            sum += value;
            ++count;
        }
        if (bytes < 0) perror("read");
        printf("analyser: %d readings, average %.1f, max %d\n",
               count, count ? (double)sum / count : 0.0, max);
        close(fd[0]);
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    int readings[] = {31, 33, 36, 34, 41, 38};
    for (size_t i = 0; i < sizeof readings / sizeof readings[0]; ++i) {
        if (write(fd[1], &readings[i], sizeof readings[i]) !=
            (ssize_t)sizeof readings[i]) {
            perror("write");
            close(fd[1]);
            waitpid(pid, NULL, 0);
            return EXIT_FAILURE;
        }
    }
    close(fd[1]); /* Closing the writer sends EOF to the child. */
    waitpid(pid, NULL, 0);
    printf("reader: done\n");
    return EXIT_SUCCESS;
}
