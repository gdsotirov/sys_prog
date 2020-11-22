/* Example    : pr5-9
 * Description: Interval timer example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

void timeout(int sig) { /* SIGALRM Handler */
    time_t time_buf;

    time(&time_buf); /* return time in seconds */
    printf("Date/Time: %s", ctime(&time_buf)); /* transform time in date */
}

int main() {
    struct sigaction act;
    struct itimerval val;

    sigemptyset(&act.sa_mask);

    act.sa_handler = timeout;
    act.sa_flags = SA_RESTART;

    if ( sigaction(SIGALRM, &act, 0) == -1 ) {
        perror("Error: Sigaction failed ");
        exit(1);
    }

    val.it_interval.tv_sec  = 1; /* recharge value in 10 seconds ... */
    val.it_interval.tv_usec = 0; /* ... and 0 micro seconds */
    val.it_value.tv_sec     = 1; /* fix value in 10 seconds */
    val.it_value.tv_usec    = 0; /* ... and 0 micro seconds */

    /* setting real interval timer */
    if ( setitimer(ITIMER_REAL, &val, 0) == -1 ) {
        perror("Error: Setitimer failed ");
        exit(2);
    }
    else for ( ;; ) {
        pause(); /* waiting for signal from timer */
    }

    return 0;
}

