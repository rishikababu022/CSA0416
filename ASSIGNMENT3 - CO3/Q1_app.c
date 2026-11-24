/* Bank branch program: links to the encryption library (statically OR dynamically,
 * decided only by how it is built).
 * Usage: ./app [maps] [sleep N]   maps = print /proc/self/maps, sleep N = stay alive N seconds */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Q1_encrypt.h"

static int g_data = 42;                       /* .data  */

static void print_status(void)
{
    FILE *f = fopen("/proc/self/status", "r"); char l[256];
    while (f && fgets(l, sizeof l, f))
        if (!strncmp(l, "VmSize", 6) || !strncmp(l, "VmRSS", 5)) fputs(l, stdout);
    if (f) fclose(f);
}

int main(int argc, char **argv)
{
    char msg[] = "ACCOUNT-1234";
    printf("pid=%d library=%s\n", getpid(), enc_version());
    encrypt_buf(msg, strlen(msg));
    printf("ciphertext (hex):");
    for (size_t i = 0; i < strlen(msg); i++) printf(" %02x", (unsigned char)msg[i]);
    printf("\n");
    printf("address of main        = %p (code of the program)\n", (void *)main);
    printf("address of encrypt_buf = %p (code of the library)\n", (void *)encrypt_buf);
    printf("address of printf      = %p (libc)\n", (void *)printf);
    printf("address of g_data      = %p (program data)\n", (void *)&g_data);
    print_status();
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "maps")) {
            printf("---- /proc/self/maps ----\n");
            FILE *f = fopen("/proc/self/maps", "r"); char l[512];
            while (f && fgets(l, sizeof l, f)) fputs(l, stdout);
            if (f) fclose(f);
        } else if (!strcmp(argv[i], "sleep") && i + 1 < argc) {
            fflush(stdout); sleep((unsigned)atoi(argv[++i]));
        }
    }
    return 0;
}
