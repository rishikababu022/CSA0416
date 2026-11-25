/* Trigger a segmentation fault: the MMU finds no valid page-table entry for the address (or a protection
 * violation), raises a trap, the kernel finds no VMA that allows the access and sends SIGSEGV.
 * Usage: ./Q3_segfault null | write | stack | handler */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#pragma GCC diagnostic ignored "-Winfinite-recursion"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
static void on_segv(int sig, siginfo_t *si, void *ctx)
{
    (void)ctx; char b[128];
    int n = snprintf(b, sizeof b, "caught signal %d (SIGSEGV), faulting address %p, si_code=%s\n", sig, si->si_addr,
                     si->si_code == SEGV_MAPERR ? "SEGV_MAPERR (no mapping)" : "SEGV_ACCERR (permission)");
    write(2, b, (size_t)n); _exit(139);
}
static void recurse(int d) { volatile char big[65536]; big[0] = (char)d; recurse(d + 1); big[1] = 0; }
int main(int argc, char **argv)
{
    const char *m = argc > 1 ? argv[1] : "null";
    if (argc > 2 || !strcmp(m, "handler")) {
        struct sigaction sa; memset(&sa, 0, sizeof sa); sa.sa_sigaction = on_segv; sa.sa_flags = SA_SIGINFO; sigaction(SIGSEGV, &sa, NULL);
        if (!strcmp(m, "handler")) m = "null"; else m = argv[2];
        if (argc > 2) m = argv[2];
    }
    if (!strcmp(m, "null"))  { volatile int *p = NULL; printf("writing to NULL...\n"); fflush(stdout); *p = 1; }
    if (!strcmp(m, "write")) { char *s = (char *)"literal"; printf("writing into read-only string literal...\n"); fflush(stdout); s[0] = 'X'; }
    if (!strcmp(m, "stack")) { printf("infinite recursion, stack overflow...\n"); fflush(stdout); recurse(0); }
    return 0;
}
