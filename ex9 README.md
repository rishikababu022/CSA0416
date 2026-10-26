#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int id;
    char *data;

    id = shmget(IPC_PRIVATE, 100, IPC_CREAT | 0666);

    if (id == -1) {
        perror("shmget");
        return 1;
    }

    data = (char *)shmat(id, NULL, 0);

    if (data == (char *)-1) {
        perror("shmat");
        shmctl(id, IPC_RMID, NULL);
        return 1;
    }

    strcpy(data, "Hello from shared memory!");
    printf("Shared data: %s\n", data);

    shmdt(data);
    shmctl(id, IPC_RMID, NULL);

    return 0;
}<img width="625" height="82" alt="Screenshot 2026-10-26 223047" src="https://github.com/user-attachments/assets/936185c7-7220-43f5-97d7-db48a2e0f2da" />
