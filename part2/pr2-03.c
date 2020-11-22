/* Example    : pr2-3
 * Description: Demonstrates wait and waitpid system calls.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
    pid_t childpid;
    int status;

    switch ( childpid = fork() ) {
        case -1 :   perror("Error: Fork failed "); /* unsuccessful */
                    break;
        case  0 :   /* This is a child process that does nothing */
                    printf("Child process.\n");
                    while ( 1 )
                        ;
                    _exit(0);
        default :   printf("Parent after fork\n");
                    /* parent id blocked */
                    while ( wait(&status) != childpid )
                        ;
                    /* checking status with macros */
                    printf("Child PID: %d. ", childpid);
                    if ( WIFEXITED(status) )
                        printf("Terminated. Exit status: %d\n", WEXITSTATUS(status));
                    else if ( WIFSIGNALED(status) )
                        printf("Killed. Signal: %d\n", WTERMSIG(status));
                    else if ( WIFSTOPPED(status) )
                        printf("Stopped. Signal: %d\n", WSTOPSIG(status));
                    else perror("Error: Wait failed ");

                    _exit(0);
    }

    return 1;
}

