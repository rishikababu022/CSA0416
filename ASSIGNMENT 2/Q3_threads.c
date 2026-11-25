#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAMERAS 64
#define WORK_PER_CAMERA 250000L

typedef struct {
    int first;
    int last;
    unsigned long long result;
} Job;

static unsigned long long process_camera(int camera) {
    unsigned long long value = (unsigned long long)camera + 1U;
    for (long i = 0; i < WORK_PER_CAMERA; ++i)
        value = (value * 1664525ULL + 1013904223ULL) % 1000000007ULL;
    return value;
}

static void *worker(void *arg) {
    Job *job = (Job *)arg;
    unsigned long long sum = 0;
    for (int camera = job->first; camera < job->last; ++camera)
        sum += process_camera(camera);
    job->result = sum;
    return NULL;
}

static double elapsed_seconds(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    struct timespec begin, end;
    unsigned long long sequential_sum = 0;

    clock_gettime(CLOCK_MONOTONIC, &begin);
    for (int camera = 0; camera < CAMERAS; ++camera)
        sequential_sum += process_camera(camera);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Sequential: %.6f seconds, checksum=%llu\n",
           elapsed_seconds(begin, end), sequential_sum);

    const int pool_sizes[] = {4, 8, 16};
    for (size_t s = 0; s < sizeof pool_sizes / sizeof pool_sizes[0]; ++s) {
        int count = pool_sizes[s];
        pthread_t threads[16];
        Job jobs[16];

        clock_gettime(CLOCK_MONOTONIC, &begin);
        for (int i = 0; i < count; ++i) {
            jobs[i].first = i * CAMERAS / count;
            jobs[i].last = (i + 1) * CAMERAS / count;
            jobs[i].result = 0;
            if (pthread_create(&threads[i], NULL, worker, &jobs[i]) != 0) {
                perror("pthread_create");
                return EXIT_FAILURE;
            }
        }

        unsigned long long total = 0;
        for (int i = 0; i < count; ++i) {
            pthread_join(threads[i], NULL);
            total += jobs[i].result;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("%2d threads: %.6f seconds, checksum=%llu\n",
               count, elapsed_seconds(begin, end), total);
    }
    return EXIT_SUCCESS;
}
