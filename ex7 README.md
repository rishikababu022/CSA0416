#include <stdio.h>

int main() {
    int n, i, j, time = 0, done = 0, best;
    int at[10], bt[10], ct[10], wt[10], tat[10], used[10] = {0};

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter arrival and burst time of P%d: ", i + 1);
        scanf("%d%d", &at[i], &bt[i]);
    }

    while (done < n) {
        best = -1;

        for (i = 0; i < n; i++) {
            if (!used[i] && at[i] <= time &&
                (best == -1 || bt[i] < bt[best]))
                best = i;
        }

        if (best == -1) {
            time++;
            continue;
        }

        time += bt[best];
        ct[best] = time;
        used[best] = 1;
        done++;
    }

    printf("\nP\tWT\tTAT\n");
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        printf("P%d\t%d\t%d\n", i + 1, wt[i], tat[i]);
    }

    return 0;
}<img width="475" height="312" alt="Screenshot 2026-10-26 222143" src="https://github.com/user-attachments/assets/35bc15ae-23d8-48f3-980f-091ed8018f77" />
