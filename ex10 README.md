#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long type;
    char text[100];
};

int main() {
    int id;
    struct message msg;

    id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
   if (id == -1) {
        perror("msgget");
        return 1;
    }

    msg.type = 1;
    strcpy(msg.text, "Hello Message Queue!");

    msgsnd(id, &msg, sizeof(msg.text), 0);
    printf("Message sent: %s\n", msg.text);

    msgrcv(id, &msg, sizeof(msg.text), 1, 0);
    printf("Message received: %s\n", msg.text);

    msgctl(id, IPC_RMID, NULL);

    return 0;
}
 <img width="462" height="141" alt="Screenshot 2026-10-26 223607" src="https://github.com/user-attachments/assets/f4515b28-c271-4935-bcd3-ff2b0f7adfb7" />
