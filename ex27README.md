#include <stdio.h>
#include <dirent.h>

int main()
{
    DIR *dir;

    struct dirent *entry;

    dir = opendir(".");

    if(dir == NULL) {
        printf("Unable to open directory\n");
        return 1;
    }

    printf("Files and Directories:\n");

    while((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);

    return 0;
}<img width="275" height="345" alt="Screenshot 2026-11-25 225643" src="https://github.com/user-attachments/assets/4c97e42c-8aca-4c75-be51-172f7f3cf220" />
<img width="275" height="345" alt="Screenshot 2026-11-25 225643" src="https://github.com/user-attachments/assets/58a46f83-0517-45aa-9e84-7c390d8a2896" />
