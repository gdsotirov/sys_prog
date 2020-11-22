/* Example    : pr3-7
 * Description: Pipe system call example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SZ 20

int main(int argc, char *argv[]) {
    int pipe_fd1[2];
    int pipe_fd2[2];
    int status;
    pid_t child_pid;
    char child_buf[BUF_SZ]; /* message buffers */
    char parent_buf[BUF_SZ];

    /* create two pipes */
    if ( (pipe(pipe_fd1) == -1) || (pipe(pipe_fd2) == -1) ) {
        fprintf(stderr, "%s: Can't create pipe.\n", argv[0]);
        exit(1);
    }

    /* create child process */
    if ( (child_pid = fork()) == -1 ) {
        fprintf(stderr, "%s: Can't create child process.\n", argv[0]);
        exit(2);
    }

    if ( child_pid == 0 ) {
        /* close unused file descriptors, this not affect the speparate
         * parent process */
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);

        /* read from pipe 1 */
        read(pipe_fd1[0], child_buf, BUF_SZ);
        printf("Child %d read: %s\n", getpid(), child_buf);

        /* write to pipe 2 */
        write(pipe_fd2[1], "A child message #1.\0", BUF_SZ);
        sleep(1); /* work simulation */

        write(pipe_fd2[1], "A child message #2.\0", BUF_SZ);

        /* used file descriptors are closed when all processes close it
         * file descriptors */
        close(pipe_fd1[0]);
        close(pipe_fd2[1]);

        exit(0);
    }
    else { /* parent process */
        /* close unused file descriptors, this not affect separate
         * child process */
        close(pipe_fd1[0]);
        close(pipe_fd2[1]);

        /* write to pipe 1 */
        write(pipe_fd1[1], "A parent message.\0", BUF_SZ);

        /* read from pipe 2 */
        while ( read(pipe_fd2[0], parent_buf, BUF_SZ) )
            printf("Parent %d read: %s\n", getpid(), parent_buf);

        /* unclosed file descriptors are closed, which don't affects the
         * child process */
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);

        /* wait for the child process to terminate */
        if ( waitpid(child_pid, &status, 0) == child_pid && WIFEXITED(status) )
            return WEXITSTATUS(status);

        return 2;
    }
}

