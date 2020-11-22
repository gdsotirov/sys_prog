/* Example    : pr2-5
 * Description: Process information example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid;

    /* parent process */
    printf("This is the parent process. PID is: %d\n", getpid());
    printf("   Process PPID is         : %d\n", getppid());
    printf("   Process PGID is         : %d\n", getpgid(0));
    printf("   Process SID is          : %d\n", getsid(0));
    printf("   Process real UID is     : %d\n", getuid());
    printf("   Process real GID is     : %d\n", getgid());
    printf("   Process effective UID is: %d\n", geteuid());
    printf("   Process effective GID is: %d\n", getegid());

    pid = fork();  /* create child process */
    /* child process */
    if ( pid == 0 ) {
        printf("This is the child process. PID is: %d\n", getpid());
        printf("   Process PPID is         : %d\n", getppid());
        printf("   Process PGID is         : %d\n", getpgid(0));
        printf("   Process SID is          : %d\n", getsid(0));
        printf("   Process real UID is     : %d\n", getuid());
        printf("   Process real GID is     : %d\n", getgid());
        printf("   Process effective UID is: %d\n", geteuid());
        printf("   Process effective GID is: %d\n\n", getegid());
        printf("Child will try to exec LS...\n");
        if ( execl("/bin/ls", "ls", NULL) == -1)
            printf("Execl failed! Error %d\n", errno);
    }

    if ( pid > 0 ) {
        printf("Parent waits child, returned pid: %d\n", wait(&status));
        printf("Child status is: %d\n", status);
    }

    return 0;
}

