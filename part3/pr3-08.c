/* Example    : pr3-8
 * Description: Shell command pipes example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];     /* pipe file descriptors        */
    int status[2];      /* status of finished proceses  */
    pid_t pid1;         /* process identificators       */
    pid_t pid2;

    if ( pipe(pipe_fd) == -1 ) { /* create pipe */
        perror("Error: Can't create pipe ");
        exit(1);
    }

    printf("Executing command: who | wc -l\n");

    switch ( pid1 = fork() ) { /* process for execute command "who" */
        case -1 :   perror("Error: Fork failed ");
                    exit(2);
        case  0 :   /* redirect stdout to write part */
                    if ( dup2(pipe_fd[1], STDOUT_FILENO) == -1 ) {
                        perror("Error: Dup2 failed ");
                        exit(3);
                    }

                    close(pipe_fd[0]);
                    close(pipe_fd[1]);

                    /* execute command "who" */
                    execlp("who", "who", 0);
                    perror("Error: Execlp failed ");
                    exit(4); /* error */
    }

    switch ( pid2 = fork() ) { /* process for execute command "wc" */
        case -1 :   perror("Error: Fork failed ");
                    exit(5);
        case  0 :   /* redirecting std input to read part */
                    close(0);
                    dup(pipe_fd[0]);

                    close(pipe_fd[0]);
                    close(pipe_fd[1]);

                    execlp("wc", "wc", "-l", 0); /* execute command "wc -l" */
                    perror("Error: Execlp failed ");
                    exit(6); /* error */
    }

    /* parent close descriptors */
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    if ( (waitpid(pid1, &status[0], 0) != pid1) || (waitpid(pid2, &status[1], 0) != pid2) )
        perror("Error: Waitpid failed!\n");

    return (status[1] + status[2]);
}

