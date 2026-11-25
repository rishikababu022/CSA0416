#include <stdio.h>

int main()
{
    FILE *file;
    char buffer[100];

    file = fopen("example.txt", "w");

    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    fprintf(file, "Hello World!\n");
    fprintf(file, "File Management Operations\n");

    fclose(file);

    file = fopen("example.txt", "r");

    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("File contents:\n");

    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);

    return 0;
}<img width="571" height="85" alt="Screenshot 2026-11-25 225300" src="https://github.com/user-attachments/assets/9f77a401-1d59-4b97-ae60-6122435c3dbc" />
