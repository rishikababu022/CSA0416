#include <stdio.h>
#include <string.h>

#define MAX 20

struct File
{
    char name[30];
};

int main()
{
    struct File directory[MAX];
    int count = 0;
    int choice, i, found;
    char filename[30];

    while(1)
    {
        printf("\n--- SINGLE LEVEL DIRECTORY ---\n");
        printf("1. Create File\n");
        printf("2. Display Files\n");
        printf("3. Delete File\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                if(count == MAX)
                {
                    printf("Directory is full\n");
                    break;
                }

                printf("Enter filename: ");
                scanf("%29s", filename);

                found = 0;

                for(i = 0; i < count; i++)
                {
                    if(strcmp(directory[i].name, filename) == 0)
                    {
                        found = 1;
                        break;
                    }
                }

                if(found)
                {
                    printf("File already exists\n");
                }
                else
                {
                    strcpy(directory[count].name, filename);
                    count++;
                    printf("File created successfully\n");
                }
                break;

            case 2:
                if(count == 0)
                {
                    printf("Directory is empty\n");
                }
                else
                {
                    printf("\nFiles in Directory:\n");

                    for(i = 0; i < count; i++)
                    {
                        printf("%s\n", directory[i].name);
                    }
                }
                break;

            case 3:
                printf("Enter filename to delete: ");
                scanf("%29s", filename);

                found = 0;

                for(i = 0; i < count; i++)
                {
                    if(strcmp(directory[i].name, filename) == 0)
                    {
                        found = 1;

                        for(int j = i; j < count - 1; j++)
                        {
                            directory[j] = directory[j + 1];
                        }

                        count--;

                        printf("File deleted successfully\n");
                        break;
                    }
                }

                if(!found)
                    printf("File not found\n");

                break;

            case 4:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}<img width="298" height="502" alt="Screenshot 2026-11-25 234843" src="https://github.com/user-attachments/assets/2d55af3b-5582-4421-9639-9a1f57baeb78" />
