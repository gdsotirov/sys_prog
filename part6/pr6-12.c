/* Example    : pr6-12
 * Description: A bidirectional pipes example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSZ 45

int main() {
    int pipe_fd[2]; /* pipe file descriptors */
    int child_pid;
    int status;
    char child_buf[BUFSZ]; /* message buffers */
    char parent_buf[BUFSZ];

    /* create named pipe */
    if ( pipe(pipe_fd) == -1 ) {
        fprintf(stderr, "Error: Can't create pipe!\n");
        exit(1);
    }

    if ( (child_pid = fork()) == -1 ) {
        fprintf(stderr, "Error: Can't create new process!\n");
        exit(2);
    }

    if ( child_pid == 0 ) {
        close(pipe_fd[0]); /* close unused descriptor */

        sleep(1);
        read(pipe_fd[1], child_buf, sizeof(child_buf)); /* read from pipe */
        printf("Child %d read: %s\n", getpid(), child_buf);

        write(pipe_fd[1], "This is a child message #1 to the parent!\0", BUFSZ);
        sleep(1); /* work simulation */
        write(pipe_fd[1], "This is a child message #2 to the parent!\0", BUFSZ);

        close(pipe_fd[1]); /* close used descriptor */

        exit(0);
    }
    else {
        close(pipe_fd[1]); /* close unused file descriptor */

        write(pipe_fd[0], "This is a parent message #1 to the child!\0", BUFSZ);

        while ( read(pipe_fd[0], parent_buf, sizeof(parent_buf)) )
            printf("Parent %d read: %s\n", getpid(), parent_buf);

        close(pipe_fd[0]); /* close used descriptor */

        if ( waitpid(child_pid, &status, 0) == child_pid && WIFEXITED(status) )
            return WEXITSTATUS(status);

        return 2;
    }
}

