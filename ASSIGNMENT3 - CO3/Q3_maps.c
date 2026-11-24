/* Show the segments of a running Linux program: code, data, bss, heap, shared libs, stack. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int initialized = 7;      /* .data */
int uninitialized;        /* .bss  */
const char ro[] = "read-only"; /* .rodata */
static void fn(void) {}
int main(void)
{
    int local = 0; void *heap = malloc(1 << 20);
    printf("pid %d\ncode  (fn)            %p\nrodata            %p\ndata  (initialized)   %p\nbss   (uninit)        %p\nheap  (malloc 1MB)    %p\nstack (local)         %p\n\n",
           getpid(), (void *)fn, (void *)ro, (void *)&initialized, (void *)&uninitialized, heap, (void *)&local);
    char cmd[64]; snprintf(cmd, sizeof cmd, "cat /proc/%d/maps", getpid()); fflush(stdout); system(cmd);
    free(heap); return 0;
}
