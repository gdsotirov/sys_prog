/* Example    : pr5-3
 * Description: Signal options example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    sigset_t sigmask;

    sigemptyset(&sigmask); /* initialization */

    if ( sigprocmask(0, 0, &sigmask) == -1 ) { /* read mask */
        perror("Error: Sigprocmask failed ");
        exit(1);
    }

    if ( sigismember(&sigmask, SIGINT) == 0 ) /* check if set SIGINT */
        sigaddset(&sigmask, SIGINT); /* setting SIGINT */
    if ( sigismember(&sigmask, SIGKILL) )
        sigdelset(&sigmask, SIGKILL); /* removing SIGKILL from mask */
    if ( sigprocmask(SIG_SETMASK, &sigmask, 0) == -1 ) {
        perror("Error: Sigprocmask failed ");
        exit(2);
    }

    return 0;
}

