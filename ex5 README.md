#include <stdio.h>

int main() {
    int n, i, j, temp;
    int bt[10], p[10], pr[10], wt[10], tat[10];
    float avgwt = 0, avgtat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        p[i] = i + 1;

        printf("Enter burst time of P%d: ", p[i]);
        scanf("%d", &bt[i]);

        printf("Enter priority of P%d: ", p[i]);
        scanf("%d", &pr[i]);
    }

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (pr[i] > pr[j]) {
                temp = pr[i];
                pr[i] = pr[j];
                pr[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    wt[0] = 0;

    for (i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    printf("\nProcess\tBT\tPriority\tWT\tTAT\n");

    for (i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
        avgwt += wt[i];
        avgtat += tat[i];

        printf("P%d\t%d\t%d\t\t%d\t%d\n",
               p[i], bt[i], pr[i], wt[i], tat[i]);
    }

    printf("Average WT = %.2f\n", avgwt / n);
    printf("Average TAT = %.2f\n", avgtat / n);

    return 0;
}
<img width="447" height="455" alt="Screenshot 2026-10-26 221016" src="https://github.com/user-attachments/assets/b8a1721c-4404-4451-ac31-fbe25a5a6951" />
