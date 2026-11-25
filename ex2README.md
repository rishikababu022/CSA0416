#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd1, fd2, n;
    char source[100], destination[100];
    char buffer[1024];

    printf("Enter source filename: ");
    scanf("%99s", source);

    printf("Enter destination filename: ");
    scanf("%99s", destination);

    fd1 = open(source, O_RDONLY);

    if(fd1 == -1)
    {
        perror("Source file error");
        return 1;
    }

    fd2 = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(fd2 == -1)
    {
        perror("Destination file error");
        close(fd1);
        return 1;
    }

    while((n = read(fd1, buffer, sizeof(buffer))) > 0)
    {
        int total = 0;

        while(total < n)
        {
            int written = write(fd2, buffer + total, n - total);

            if(written == -1)
            {
                perror("Write error");
                close(fd1);
                close(fd2);
                return 1;
            }

            total += written;
        }
    }

    if(n == -1)
        perror("Read error");
    else
        printf("File copied successfully.\n");

    close(fd1);
    close(fd2);

    return n == -1 ? 1 : 0;
}
