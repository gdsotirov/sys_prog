/* Example    : pr5-8
 * Description: Demonstrates alarm system call.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>

void timeout(int sig) { /* SIGALRM Handler */
    time_t time_buf;

    time(&time_buf); /* return time in seconds */
    printf("Date/Time: %s", ctime(&time_buf)); /* transform time in date */

    alarm(1); /* will send signal after 10 seconds */
}

int main() {
    struct sigaction act;

    sigemptyset(&act.sa_mask);

    act.sa_handler = timeout;
    act.sa_flags = SA_RESTART;

    if ( sigaction(SIGALRM, &act, 0) == -1 ) {
        perror("Error: Sigaction failed ");
        exit(1);
    }

    if ( alarm(1) == -1 )
        perror("Error: Alarm failed ");
    else for ( ;; ) {
        pause();
    }

    return 0;
}

