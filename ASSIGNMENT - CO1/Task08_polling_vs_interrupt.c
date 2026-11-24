/* Task 8: polling vs interrupt (signal) detection of an abnormal reading.
   A "sensor" (SIGALRM timer) raises an abnormal flag at a random time. Polling checks every P ms;
   the interrupt handler runs immediately. Measures detection delay and CPU time used. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
static volatile sig_atomic_t abnormal = 0;
static struct timespec t_event, t_seen;
static double ms(struct timespec a, struct timespec b){return (b.tv_sec-a.tv_sec)*1e3+(b.tv_nsec-a.tv_nsec)/1e6;}
static void sensor(int s){ (void)s; clock_gettime(CLOCK_MONOTONIC,&t_event); abnormal = 1; }
static void sensor_irq(int s){ (void)s; clock_gettime(CLOCK_MONOTONIC,&t_event); clock_gettime(CLOCK_MONOTONIC,&t_seen); abnormal = 1; }
static double cpu(void){struct rusage r;getrusage(RUSAGE_SELF,&r);return r.ru_utime.tv_sec+r.ru_stime.tv_sec+(r.ru_utime.tv_usec+r.ru_stime.tv_usec)/1e6;}
int main(void)
{
    int polls[] = {100, 10, 1};
    for (int k = 0; k < 3; k++) {
        double worst = 0, c0 = cpu();
        for (int trial = 0; trial < 20; trial++) {
            abnormal = 0; signal(SIGALRM, sensor);
            struct itimerval it = {{0,0},{0, 50000 + (rand()%50000)}};   /* event after 50-100 ms */
            setitimer(ITIMER_REAL, &it, NULL);
            while (!abnormal) { struct timespec p = {0, polls[k]*1000000L}; nanosleep(&p, NULL); if (abnormal) break; }
            clock_gettime(CLOCK_MONOTONIC,&t_seen);
            double d = ms(t_event, t_seen); if (d > worst) worst = d;
        }
        printf("polling every %3d ms: worst delay %7.3f ms, CPU %.4f s\n", polls[k], worst, cpu()-c0);
    }
    double worst = 0, c0 = cpu();
    for (int trial = 0; trial < 20; trial++) {
        abnormal = 0; signal(SIGALRM, sensor_irq);
        struct itimerval it = {{0,0},{0, 50000 + (rand()%50000)}};
        setitimer(ITIMER_REAL, &it, NULL);
        pause();                                   /* CPU idle until the interrupt */
        double d = ms(t_event, t_seen); if (d > worst) worst = d;
    }
    printf("interrupt (signal)  : worst delay %7.3f ms, CPU %.4f s\n", worst, cpu()-c0);
    return 0;
}
