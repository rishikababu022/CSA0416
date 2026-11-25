<img width="382" height="233" alt="image" src="https://github.com/user-attachments/assets/ad2926dc-00b3-47bc-ad1d-dc9b6aedc7b2" />
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, head, i, j, seek = 0;
    int size = 200;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    int a[n];

    printf("Enter request queue:\n");

    for(i=0; i<n; i++)
        scanf("%d", &a[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    for(i=0; i<n-1; i++)
    {
        for(j=0; j<n-i-1; j++)
        {
            if(a[j] > a[j+1])
            {
                int temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }

    printf("\nHead Movement: %d", head);

    for(i=0; i<n; i++)
    {
        if(a[i] >= head)
        {
            seek += abs(head-a[i]);
            head = a[i];
            printf(" -> %d", head);
        }
    }

    seek += abs(head-(size-1));
    head = size-1;

    printf(" -> %d", head);

    seek += size-1;
    head = 0;

    printf(" -> %d", head);

    for(i=0; i<n; i++)
    {
        if(a[i] < head || a[i] == 0)
        {
            seek += abs(head-a[i]);
            head = a[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal Seek Time: %d\n", seek);

    return 0;
}
