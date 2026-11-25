#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

int main()
{
    int fd;

    fd = open("foo.txt", O_CREAT | O_RDWR, 0644);

    if(fd == -1) {
        perror("open");
        return 1;
    }

    write(fd, "UNIX I/O calls\n", 14);

    printf("File descriptor: %d\n", fd);

    printf("File flags: %d\n",
           fcntl(fd, F_GETFL));

    printf("Current offset: %ld\n",
           (long)lseek(fd, 0, SEEK_CUR));

    printf("Offset after seek: %ld\n",
           (long)lseek(fd, 0, SEEK_SET));

    close(fd);

    struct stat st;

    if(stat("foo.txt", &st) == 0)
        printf("File size: %ld bytes\n",
               (long)st.st_size);

    DIR *dp = opendir(".");

    if(dp == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;

    printf("Directory entries:\n");

    while((entry = readdir(dp)) != NULL)
        printf("%s\n", entry->d_name);

    closedir(dp);

    return 0;
}
