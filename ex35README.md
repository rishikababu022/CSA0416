#include <stdio.h>

int main()
{
    int index[20], n, i;

    printf("Enter number of blocks: ");
    scanf("%d", &n);

    printf("Enter block numbers:\n");

    for(i=0; i<n; i++)
    {
        scanf("%d", &index[i]);
    }

    printf("\nIndex Block:\n");

    for(i=0; i<n; i++)
    {
        printf("%d ", index[i]);
    }

    printf("\n\nFile Block Allocation:\n");

    for(i=0; i<n; i++)
    {
        printf("File Block %d -> Disk Block %d\n",
               i+1, index[i]);
    }

    return 0;
}<img width="455" height="372" alt="Screenshot 2026-11-25 232224" src="https://github.com/user-attachments/assets/78a3eb37-a088-4a87-835b-c2d2c23bec81" />
