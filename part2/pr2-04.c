/* Example    : pr2-4
 * Description: Demonstrates execl system call.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

int sys_func(const char *string);

int main() {
    if ( sys_func("who") == -1 ) {
        perror("Error: sys_func failed ");
        return -1;
    }

    return 0;
}

int sys_func(const char *string) {
    pid_t childpid;
    pid_t pid;
    int status;

    if ( (childpid = fork()) == -1 ) {
        perror("Error: Fork failed ");
        return -1;
    }

    if ( childpid == 0 ) {
        execl("/bin", "bsh", "-c", string, 0);
        perror("Error: Execl failed ");
        return errno;
    }

    pid = waitpid(childpid, &status, 0);

    if ( WIFEXITED(status) && pid == childpid )
        return WEXITSTATUS(status);
    perror("Error: Waitpid failed ");

    return -1;
}

