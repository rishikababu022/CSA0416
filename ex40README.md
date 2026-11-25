#include <stdio.h>
#include <sys/stat.h>

int main()
{
    struct stat st;
    FILE *fp;

    fp = fopen("sample.txt", "w");

    if(fp == NULL)
    {
        printf("File creation failed\n");
        return 1;
    }

    fprintf(fp, "Linux File Permissions");
    fclose(fp);

    chmod("sample.txt", 0644);

    if(stat("sample.txt", &st) == 0)
    {
        printf("File created successfully\n");

        printf("File Permissions: ");

        printf((st.st_mode & S_IRUSR) ? "r" : "-");
        printf((st.st_mode & S_IWUSR) ? "w" : "-");
        printf((st.st_mode & S_IXUSR) ? "x" : "-");

        printf((st.st_mode & S_IRGRP) ? "r" : "-");
        printf((st.st_mode & S_IWGRP) ? "w" : "-");
        printf((st.st_mode & S_IXGRP) ? "x" : "-");

        printf((st.st_mode & S_IROTH) ? "r" : "-");
        printf((st.st_mode & S_IWOTH) ? "w" : "-");
        printf((st.st_mode & S_IXOTH) ? "x" : "-");

        printf("\n");
    }

    return 0;
}<img width="325" height="187" alt="Screenshot 2026-11-25 233426" src="https://github.com/user-attachments/assets/337dc731-5be7-482c-90e7-94e810f9a9e3" />
