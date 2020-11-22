/* Example    : pr2-1
 * Description: Demonstrates fork system call.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    pid = fork(); /* process creates child process */

    if ( pid < (pid_t)0 ) /* Error, can't create process */
        perror("Error: Fork failed ");
    if ( pid > (pid_t)0 ) /* fork() returned child pit to parrent */
        printf("The parent is running.\n"); /* executed from the parent */
    if ( pid == (pid_t)0 ) /* fork() returned 0 to the child process */
        printf("The child is running.\n"); /* executed from the child */

    return 0;
}

