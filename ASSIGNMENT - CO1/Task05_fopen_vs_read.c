/* Task 5: system calls of fopen/fread/fwrite (library buffering) vs read()/write() directly.
   Run each mode under strace -c to compare:  ./a.out lib  |  ./a.out sys 512 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "lib";
    size_t b = argc > 2 ? (size_t)atol(argv[2]) : 512;
    char *buf = malloc(65536);
    FILE *f = fopen("t5_src.dat", "w");
    for (int i = 0; i < 100000; i++) fputc('A' + i % 26, f);
    fclose(f);
    if (strcmp(mode, "lib") == 0) {
        FILE *in = fopen("t5_src.dat", "r"), *out = fopen("t5_dst.dat", "w");
        size_t n;
        while ((n = fread(buf, 1, 512, in)) > 0) fwrite(buf, 1, n, out);   /* 512-byte app buffer, 4096-byte stdio buffer */
        fclose(in); fclose(out);
    } else {
        int in = open("t5_src.dat", O_RDONLY), out = open("t5_dst.dat", O_WRONLY|O_CREAT|O_TRUNC, 0644);
        ssize_t n;
        while ((n = read(in, buf, b)) > 0) write(out, buf, n);
        close(in); close(out);
    }
    unlink("t5_src.dat"); unlink("t5_dst.dat"); free(buf);
    return 0;
}
