#include <stdio.h>

int main() {
    int n, i, q, time = 0, done;
    int bt[10], rt[10], wt[10], tat[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Burst time of P%d: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &q);

    do {
        done = 1;

        for (i = 0; i < n; i++) {
            if (rt[i] > 0) {
                done = 0;

                if (rt[i] > q) {
                    time += q;
                    rt[i] -= q;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    tat[i] = time;
                    wt[i] = tat[i] - bt[i];
                }
            }
        }
    } while (!done);

    printf("\nP\tBT\tWT\tTAT\n");
    for (i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\n",
               i + 1, bt[i], wt[i], tat[i]);

    return 0;
}#include <stdio.h>

int main() {
    int n, i, q, time = 0, done;
    int bt[10], rt[10], wt[10], tat[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Burst time of P%d: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &q);

    do {
        done = 1;

        for (i = 0; i < n; i++) {
            if (rt[i] > 0) {
                done = 0;

                if (rt[i] > q) {
                    time += q;
                    rt[i] -= q;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    tat[i] = time;
                    wt[i] = tat[i] - bt[i];
                }
            }
        }
    } while (!done);

    printf("\nP\tBT\tWT\tTAT\n");
    for (i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\n",
               i + 1, bt[i], wt[i], tat[i]);

    return 0;
}<img width="337" height="320" alt="Screenshot 2026-10-26 222644" src="https://github.com/user-attachments/assets/069cdfd4-0d68-4796-8f81-f21085172c23" />
