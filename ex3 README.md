<img width="440" height="357" alt="Screenshot 2026-10-26 213129" src="https://github.com/user-attachments/assets/06407d1c-a4c7-4901-8bc5-bd17af4a1784" />
#include <stdio.h>

int main()
{
    int bt[20], wt[20], tat[20];
    int n, i;
    float avgwt = 0, avgtat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst times:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    wt[0] = 0;

    for (i = 1; i < n; i++)
    {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    for (i = 0; i < n; i++)
    {
        tat[i] = wt[i] + bt[i];
        avgwt += wt[i];
        avgtat += tat[i];
    }

    printf("\nProcess\tBT\tWT\tTAT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\n",
               i + 1, bt[i], wt[i], tat[i]);
    }

    printf("Average Waiting Time = %.2f\n",
           avgwt / n);
    printf("Average Turnaround Time = %.2f\n",
           avgtat / n);

    return 0;
}
