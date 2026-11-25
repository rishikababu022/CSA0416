#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int fd, n;
    char buffer[100];

    fd = creat("sample.txt", 0644);

    if(fd == -1) {
        perror("creat");
        return 1;
    }

    printf("File created successfully\n");
    close(fd);

    fd = open("sample.txt", O_WRONLY | O_APPEND);

    if(fd == -1) {
        perror("open");
        return 1;
    }

    write(fd, "Hello World!\n", 13);
    printf("Data written successfully\n");
    close(fd);

    fd = open("sample.txt", O_RDONLY);

    if(fd == -1) {
        perror("open");
        return 1;
    }

    n = read(fd, buffer, sizeof(buffer));

    printf("File contents:\n");
    write(STDOUT_FILENO, buffer, n);

    close(fd);

    remove("sample.txt");

    printf("\nFile deleted successfully\n");

    return 0;
}<img width="541" height="207" alt="Screenshot 2026-11-25 224629" src="https://github.com/user-attachments/assets/210c3fde-dea3-4408-a21f-bfcd404f274a" />
