<img width="443" height="475" alt="Screenshot 2026-10-26 214735" src="https://github.com/user-attachments/assets/8d6b26fe-0ea1-42c5-a19e-083b4f3ea4cf" />
#include <stdio.h>

int main()
{
    int bt[20], p[20], wt[20], tat[20];
    int n, i, j, temp;
    float avgwt = 0, avgtat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst times:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
        p[i] = i + 1;
    }

    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (bt[i] > bt[j])
            {
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

    for (i = 1; i < n; i++)
    {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    printf("\nProcess\tBT\tWT\tTAT\n");

    for (i = 0; i < n; i++)
    {
        tat[i] = wt[i] + bt[i];

        avgwt += wt[i];
        avgtat += tat[i];

        printf("P%d\t%d\t%d\t%d\n",
               p[i], bt[i], wt[i], tat[i]);
    }

    printf("Average Waiting Time = %.2f\n",
           avgwt / n);
    printf("Average Turnaround Time = %.2f\n",
           avgtat / n);

    return 0;
}
