/* Example    : pr5-4
 * Description: Demonstrates sigaction system call.
 * Edited by  : George D. Sotirov, astornom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termio.h>
#include <sys/wait.h>

void on_sigchld(int sig) { /* SIGCHLD Handler */
    printf("SIGCHLD handler - the parent caught the signal.\n");
}

int main() {
    int pid;
    int child_pid;
    int status;
    struct sigaction act;
    struct sigaction old_act;

    act.sa_handler = on_sigchld;
    act.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    sigemptyset(&act.sa_mask);

    if ( sigaction(SIGCHLD, &act, &old_act) == -1 ) { /* setting handler */
        perror("Error: Sigaction failed ");
        exit(1);
    }

    switch ( pid = fork() ) { /* create process */
        case -1 :   perror("Error: Fork failed ");
                    exit(2);
        case  0 :   /* child process */
                    printf("This is the child with pid = %d.\n", getpid());
                    exit(5); /* send exit code 5 to the parent */
        default :   /* parent process */
                    child_pid = wait(&status);
                    printf("Child exits: pid = %d, exit code = %d, status = %d.\n", child_pid, (status >> 8), (status & 0377));
    }

    return 0;
}

