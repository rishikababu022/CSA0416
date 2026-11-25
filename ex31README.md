#include <stdio.h>

#define MAX 3

int main()
{
    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2};
    int n = 13, frames[MAX];
    int pointer = 0, faults = 0;
    int i, j, found;

    for(i=0; i<MAX; i++)
        frames[i] = -1;

    printf("Page Replacement:\n");

    for(i=0; i<n; i++)
    {
        found = 0;

        for(j=0; j<MAX; j++)
        {
            if(frames[j] == pages[i])
            {
                found = 1;
                break;
            }
        }

        if(!found)
        {
            frames[pointer] = pages[i];
            pointer = (pointer+1)%MAX;
            faults++;
        }

        printf("%d : ", pages[i]);

        for(j=0; j<MAX; j++)
            printf("%d ", frames[j]);

        printf("\n");
    }

    printf("Total Page Faults = %d\n", faults);

    return 0;
}<img width="316" height="515" alt="Screenshot 2026-11-25 231235" src="https://github.com/user-attachments/assets/cce2022e-f048-4d6d-b4e3-6abd3cef3510" />
