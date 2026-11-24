/* Alternative concept: load the encryption library at RUN TIME with dlopen()/dlsym(),
 * only when encryption is actually needed, and handle a missing library gracefully.
 * Usage: ./app_dlopen [noenc] [maps]  (noenc = encryption not needed, library never loaded) */
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

static void print_status(const char *when)
{
    FILE *f = fopen("/proc/self/status", "r"); char l[256];
    printf("[%s] ", when);
    while (f && fgets(l, sizeof l, f))
        if (!strncmp(l, "VmSize", 6)) { l[strcspn(l, "\n")] = 0; printf("%s  ", l); }
        else if (!strncmp(l, "VmRSS", 5)) { l[strcspn(l, "\n")] = 0; printf("%s", l); }
    printf("\n");
    if (f) fclose(f);
}

int main(int argc, char **argv)
{
    int need = 1, maps = 0;
    for (int i = 1; i < argc; i++) { if (!strcmp(argv[i], "noenc")) need = 0; if (!strcmp(argv[i], "maps")) maps = 1; }
    print_status("before dlopen");
    if (!need) { printf("encryption not needed: library NOT loaded\n"); return 0; }

    void *h = dlopen("./libencrypt.so", RTLD_LAZY);
    if (!h) {                                   /* error handling for a missing library */
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        fprintf(stderr, "-> continuing in degraded mode: transaction refused, nothing sent in clear text\n");
        return 2;
    }
    void (*enc)(char *, size_t) = (void (*)(char *, size_t))dlsym(h, "encrypt_buf");
    const char *(*ver)(void)    = (const char *(*)(void))dlsym(h, "enc_version");
    if (!enc || !ver) { fprintf(stderr, "dlsym failed: %s\n", dlerror()); dlclose(h); return 3; }

    char msg[] = "ACCOUNT-1234";
    enc(msg, strlen(msg));
    printf("loaded library %s, ciphertext:", ver());
    for (size_t i = 0; i < strlen(msg); i++) printf(" %02x", (unsigned char)msg[i]);
    printf("\naddress of encrypt_buf (from dlsym) = %p\n", (void *)enc);
    print_status("after dlopen ");
    if (maps) {
        printf("---- /proc/self/maps ----\n");
        FILE *f = fopen("/proc/self/maps", "r"); char l[512];
        while (f && fgets(l, sizeof l, f)) fputs(l, stdout);
        if (f) fclose(f);
    }
    dlclose(h);
    return 0;
}
