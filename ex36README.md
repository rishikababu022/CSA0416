#include <stdio.h>

int main()
{
    int block[20], n, i;

    printf("Enter number of blocks: ");
    scanf("%d", &n);

    printf("Enter disk block numbers:\n");

    for(i=0; i<n; i++)
    {
        scanf("%d", &block[i]);
    }

    printf("\nLinked File Allocation:\n");

    printf("Start Block: %d\n", block[0]);

    for(i=0; i<n-1; i++)
    {
        printf("%d -> %d\n", block[i], block[i+1]);
    }

    printf("%d -> NULL\n", block[n-1]);

    printf("End Block: %d\n", block[n-1]);

    return 0;
}<img width="405" height="328" alt="Screenshot 2026-11-25 232405" src="https://github.com/user-attachments/assets/3a72cdfe-7e4a-4af2-9474-44f3603e6e2b" />
