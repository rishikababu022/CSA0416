/* Task 5: minimal program for strace evidence (open/read/write/close) */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    char buf[64];
    int fd = open("/etc/hostname", O_RDONLY);
    ssize_t n = read(fd, buf, sizeof buf);
    write(STDOUT_FILENO, buf, n);
    close(fd);
    return 0;
}
