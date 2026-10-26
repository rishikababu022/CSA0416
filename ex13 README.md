#include <stdio.h>

int main() {
    int b[10], p[10], alloc[10];
    int nb, np, i, j, choice, best;

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);

    for (i = 0; i < nb; i++) {
        printf("Size of block %d: ", i + 1);
        scanf("%d", &b[i]);
    }

    printf("Enter number of processes: ");
    scanf("%d", &np);

    for (i = 0; i < np; i++) {
        printf("Size of process %d: ", i + 1);
        scanf("%d", &p[i]);
        alloc[i] = -1;
    }

    printf("1.First Fit  2.Best Fit  3.Worst Fit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    for (i = 0; i < np; i++) {
        best = -1;

        for (j = 0; j < nb; j++) {
            if (b[j] >= p[i]) {
                if (choice == 1) {
                    best = j;
                    break;
                }

                if (best == -1 ||
                    (choice == 2 && b[j] < b[best]) ||
                    (choice == 3 && b[j] > b[best])) {
                    best = j;
                }
            }
        }

        if (best != -1) {
            alloc[i] = best;
            b[best] -= p[i];
        }
    }

    printf("\nProcess\tSize\tBlock\n");

    for (i = 0; i < np; i++) {
        printf("P%d\t%d\t", i + 1, p[i]);

        if (alloc[i] == -1)
            printf("Not allocated\n");
        else
            printf("%d\n", alloc[i] + 1);
    }

    return 0;
}<img width="370" height="423" alt="Screenshot 2026-10-26 230817" src="https://github.com/user-attachments/assets/3106d3b3-00a5-4f9c-b662-da36c94767a0" />
