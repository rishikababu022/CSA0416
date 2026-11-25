#include <stdio.h>

int mutex = 1;
int full = 0;
int empty = 10;
int x = 0;

void producer()
{
    --mutex;
    ++full;
    --empty;
    ++x;

    printf("Producer produces item %d\n", x);

    ++mutex;
}

void consumer()
{
    --mutex;
    --full;
    ++empty;

    printf("Consumer consumes item %d\n", x);

    --x;
    ++mutex;
}

int main()
{
    int choice;

    while(1)
    {
        printf("\n1. Producer");
        printf("\n2. Consumer");
        printf("\n3. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                if(mutex == 1 && empty != 0)
                    producer();
                else
                    printf("Buffer is full\n");
                break;

            case 2:
                if(mutex == 1 && full != 0)
                    consumer();
                else
                    printf("Buffer is empty\n");
                break;

            case 3:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}<img width="292" height="581" alt="Screenshot 2026-11-25 230756" src="https://github.com/user-attachments/assets/fb505723-dc69-4a43-9559-f40fbc3e29e6" />
