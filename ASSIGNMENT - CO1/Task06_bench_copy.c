/* Task 6/10: copy a large file with different buffer sizes; report read() calls and time. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec/1e9;}
static long copy(const char *s, const char *d, size_t bs, char *buf)
{
    int in = open(s, O_RDONLY), out = open(d, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    long calls = 0; ssize_t n;
    while ((n = read(in, buf, bs)) > 0) { calls++; if (write(out, buf, n) != n) break; }
    close(in); close(out); return calls;
}
int main(int argc, char **argv)
{
    size_t mb = argc > 1 ? (size_t)atol(argv[1]) : 16;
    char *buf = malloc(1<<20);
    FILE *f = fopen("bench_src.dat", "w");
    for (size_t i = 0; i < mb; i++) { for (int j = 0; j < (1<<20); j++) buf[j] = 'A' + (j % 26); fwrite(buf, 1, 1<<20, f); }
    fclose(f);
    size_t sizes[] = {1, 16, 64, 512, 4096, 65536, 1<<20};
    printf("file = %zu MB\n%10s %12s %10s\n", mb, "buffer(B)", "read calls", "time(s)");
    for (int i = 0; i < 7; i++) {
        if (sizes[i] == 1 && mb > 1) { printf("%10zu %12s %10s\n", sizes[i], "skipped", "-"); continue; }
        double t0 = now(); long c = copy("bench_src.dat", "bench_dst.dat", sizes[i], buf);
        printf("%10zu %12ld %10.4f\n", sizes[i], c, now()-t0);
    }
    unlink("bench_src.dat"); unlink("bench_dst.dat"); free(buf);
    return 0;
}
