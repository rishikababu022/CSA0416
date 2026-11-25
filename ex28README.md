#include <stdio.h>
#include <string.h>

int main()
{
    FILE *fp;
    char filename[50], pattern[50];
    char line[200];

    printf("Enter filename: ");
    scanf("%49s", filename);

    printf("Enter search pattern: ");
    scanf("%49s", pattern);

    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("File not found");
        return 1;
    }

    printf("Matching lines:\n");

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        if(strstr(line, pattern) != NULL)
            printf("%s", line);
    }

    fclose(fp);

    return 0;
}<img width="397" height="117" alt="Screenshot 2026-11-25 230431" src="https://github.com/user-attachments/assets/60426adf-8cb7-4f20-9da7-3b466002ffa4" />
