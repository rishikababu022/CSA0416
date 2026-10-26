#include <stdio.h>
#include <string.h>

int main() {
    char users[5][20];
    char files[5][5][20];
    int count[5] = {0};
    int n = 0, choice, i, u;
    char user[20], file[20];

    while (1) {
        printf("\n1.Add user  2.Add file  3.Display  4.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (n == 5) {
                printf("Maximum users reached\n");
                continue;
            }

            printf("Enter username: ");
            scanf("%19s", user);

            for (i = 0; i < n; i++)
                if (strcmp(users[i], user) == 0)
                    break;

            if (i < n)
                printf("User already exists\n");
            else {
                strcpy(users[n], user);
                n++;
                printf("User created\n");
            }
        } else if (choice == 2) {
            printf("Enter username: ");
            scanf("%19s", user);

            for (u = 0; u < n; u++)
                if (strcmp(users[u], user) == 0)
                    break;

            if (u == n) {
                printf("User not found\n");
                continue;
            }

            if (count[u] == 5) {
                printf("Directory is full\n");
                continue;
            }

            printf("Enter filename: ");
            scanf("%19s", file);

            strcpy(files[u][count[u]++], file);
            printf("File added\n");
        } else if (choice == 3) {
            for (u = 0; u < n; u++) {
                printf("\nUser: %s\n", users[u]);

                for (i = 0; i < count[u]; i++)
                    printf("%s\n", files[u][i]);
            }
        } else {
            break;
        }
    }

    return 0;
}<img width="475" height="451" alt="Screenshot 2026-10-26 225735" src="https://github.com/user-attachments/assets/f52f2541-b1f1-42c1-9c90-391724103bd9" />
