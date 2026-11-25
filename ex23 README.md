#include <stdio.h>

int main()
{
    int nb, np, b[20], p[20], alloc[20];

    printf("Enter number of blocks: ");
    scanf("%d", &nb);

    printf("Enter block sizes:\n");
    for(int i=0; i<nb; i++)
        scanf("%d", &b[i]);

    printf("Enter number of processes: ");
    scanf("%d", &np);

    printf("Enter process sizes:\n");
    for(int i=0; i<np; i++) {
        scanf("%d", &p[i]);
        alloc[i] = -1;
    }

    for(int i=0; i<np; i++) {
        for(int j=0; j<nb; j++) {
            if(b[j] >= p[i]) {
                alloc[i] = j;
                b[j] -= p[i];
                break;
            }
        }
    }

    printf("\nProcess\tSize\tBlock\n");

    for(int i=0; i<np; i++) {
        if(alloc[i] != -1)
            printf("P%d\t%d\t%d\n", i+1, p[i], alloc[i]+1);
        else
            printf("P%d\t%d\tNot allocated\n", i+1, p[i]);
    }

    return 0;
}<img width="355" height="287" alt="Screenshot 2026-11-25 224337" src="https://github.com/user-attachments/assets/600ec10c-89ee-4f50-945b-6f25493d0ba5" />
