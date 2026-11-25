#include <stdio.h>
#include <stdlib.h>

struct Record
{
    int recordNumber;
    char data[256];
};

int main()
{
    FILE *file;
    struct Record record;
    int recordNumber;

    file = fopen("sequential_file.dat", "wb");

    if(file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    printf("Enter records (Enter 0 to exit):\n");

    while(1)
    {
        printf("Record Number: ");
        scanf("%d", &record.recordNumber);

        if(record.recordNumber == 0)
            break;

        printf("Data: ");
        scanf(" %255[^\n]", record.data);

        fwrite(&record, sizeof(struct Record), 1, file);
    }

    fclose(file);

    file = fopen("sequential_file.dat", "rb");

    if(file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    while(1)
    {
        int found = 0;

        printf("\nEnter record number to read (0 to exit): ");
        scanf("%d", &recordNumber);

        if(recordNumber == 0)
            break;

        rewind(file);

        while(fread(&record, sizeof(struct Record), 1, file))
        {
            printf("\nRecord Number: %d", record.recordNumber);
            printf("\nData: %s\n", record.data);

            if(record.recordNumber == recordNumber)
            {
                found = 1;
                break;
            }
        }

        if(!found)
            printf("Record not found.\n");
    }

    fclose(file);

    return 0;
}<img width="326" height="82" alt="Screenshot 2026-11-25 232023" src="https://github.com/user-attachments/assets/2d5ff623-24db-4213-9a88-52a824e6ed7e" />
