/* Examples   : pr2-2
 * Description: Demonstrates exec system call.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid;

    pid = fork();
    /* can't create process - error */
    if ( pid < (pid_t)0 )
        perror("Error: Fork failed ");
    /* fork() returned pid on child to parent */
    /* executed from parent */
    if ( pid > (pid_t)0 )
        printf("The parent is running.\n");
    /* fork() returned 0 to child */
    if ( pid == (pid_t)0 ) {
        /* executed from the child */
        printf("The child will try to run command \"who\"...\n");
        execl("/usr/bin/who", "who", NULL);
    }

    return 0;
}

