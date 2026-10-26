#include <stdio.h>
#include <string.h>

int main() {
    char files[10][30], name[30];
    int n = 0, choice, i, found;

    while (1) {
        printf("\n1.Create file  2.Display files  3.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (n == 10) {
                printf("Directory is full\n");
                continue;
            }

            printf("Enter filename: ");
            scanf("%29s", name);

            found = 0;
            for (i = 0; i < n; i++)
                if (strcmp(files[i], name) == 0)
                    found = 1;

            if (found)
                printf("File already exists\n");
            else {
                strcpy(files[n++], name);
                printf("File created\n");
            }
        } else if (choice == 2) {
            printf("Files in directory:\n");
            for (i = 0; i < n; i++)
                printf("%s\n", files[i]);
        } else {
            break;
        }
    }

    return 0;
}<img width="581" height="297" alt="Screenshot 2026-10-26 225318" src="https://github.com/user-attachments/assets/ac48a227-a587-41c8-be37-5bb979146619" />
