#include <stdio.h>

#define MAX 3

int main()
{
    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2};
    int frames[MAX];
    int n = 13, faults = 0;
    int i, j, k, found, pos;

    for(i=0; i<MAX; i++)
        frames[i] = -1;

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
            pos = -1;
            int farthest = -1;

            for(j=0; j<MAX; j++)
            {
                int next = n;

                for(k=i+1; k<n; k++)
                {
                    if(frames[j] == pages[k])
                    {
                        next = k;
                        break;
                    }
                }

                if(next > farthest)
                {
                    farthest = next;
                    pos = j;
                }
            }

            frames[pos] = pages[i];
            faults++;
        }

        printf("%d : ", pages[i]);

        for(j=0; j<MAX; j++)
            printf("%d ", frames[j]);

        printf("\n");
    }

    printf("Total Page Faults = %d\n", faults);

    return 0;
}<img width="270" height="492" alt="Screenshot 2026-11-25 231653" src="https://github.com/user-attachments/assets/5e0988ce-656d-42a1-8cb3-cc81a157f177" />
