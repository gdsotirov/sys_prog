/* Example    : pr5-6
 * Description: Process interaction example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void on_sigterm(int); /* SIGTERM Handler */
void on_sigchld(int); /* SIGCHLD Handler */

int main() {
    pid_t pid;
    char s;

    switch ( pid = fork() ) { /* create process */
        case -1 :   perror("Error: Fork failed ");
                    exit(1);
        case  0 :   /* child process */
                    sleep(6); /* work simulation */
                    printf("Child is ready. Wait signal.\n");
                    pause(); /* waiting signal */
                    exit(15);
        default :   /* parent process */
                    sigset(SIGCHLD, on_sigchld);
                    sleep(1); /* work simulation */
                    printf("Kill the child? [y/n] ");
                    /* send signal to the child? */
                    while ( (s = getchar()) != 'y' )
                        ;
                    printf("Parent sent SIGTERM with result: %d.\n", kill(pid, SIGTERM));
                    sleep(1); /* work simulation */
    }

    return 0;
}

void on_sigchld(int sig) {
    int status;

    printf("Parent is interrupted by signal SIGCHLD.\n");
    wait(&status);
    if ( WIFEXITED(status) )
        printf("Child exits with status: %d.\n", WEXITSTATUS(status));
    else if ( WIFSIGNALED(status) )
        printf("Child is killed by signal: %d.\n", WTERMSIG(status));
}

void on_sigterm(int sig) {
    printf("Child caught software termination signal from parent.\n");
}

