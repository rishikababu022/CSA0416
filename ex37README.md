#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, head, seek = 0;

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int request[n];

    printf("Enter disk request queue:\n");

    for(int i=0; i<n; i++)
        scanf("%d", &request[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("\nHead Movement Sequence: %d", head);

    for(int i=0; i<n; i++)
    {
        seek += abs(head - request[i]);

        head = request[i];

        printf(" -> %d", head);
    }

    printf("\nTotal Seek Time: %d", seek);

    printf("\nAverage Seek Time: %.2f\n",
           (float)seek/n);

    return 0;
}<img width="557" height="237" alt="Screenshot 2026-11-25 232636" src="https://github.com/user-attachments/assets/9dd5dc7f-1507-487e-b77c-09103fa8019d" />
