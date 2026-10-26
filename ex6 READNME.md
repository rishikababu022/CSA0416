#include <stdio.h>

int main() {
    int n, i, time = 0, done = 0;
    int at[10], bt[10], rt[10], pr[10], ct[10];
    int wt[10], tat[10], best;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter arrival, burst, priority for P%d: ", i + 1);
        scanf("%d%d%d", &at[i], &bt[i], &pr[i]);
        rt[i] = bt[i];
    }

    while (done < n) {
        best = -1;

        for (i = 0; i < n; i++) {
            if (at[i] <= time && rt[i] > 0 &&
                (best == -1 || pr[i] < pr[best]))
                best = i;
        }

        if (best == -1) {
            time++;
            continue;
        }

        rt[best]--;
        time++;

        if (rt[best] == 0) {
            ct[best] = time;
            done++;
        }
    }

    printf("\nP\tWT\tTAT\n");
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        printf("P%d\t%d\t%d\n", i + 1, wt[i], tat[i]);
    }

    return 0;
}<img width="556" height="397" alt="Screenshot 2026-10-26 221850" src="https://github.com/user-attachments/assets/27143e17-4788-4e49-a278-92bbf93eeb02" />
