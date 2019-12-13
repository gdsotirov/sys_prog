/* Example    : pr5-5
 * Description: Demonstrates sigsetjmp and siglongjmp system calls.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

sigjmp_buf ctx;

void on_sigint(int sig) { /* SIGINT Handler */
    printf("Enter on_sigint. Now call siglongjmp\n");
    siglongjmp(ctx, 1); /* jumping with val = 1 */
}

int main() {
    sigset_t sigmask;
    int retcode;
    struct sigaction act;

    sigemptyset(&sigmask); /* initialization */

    if ( (sigaddset(&sigmask, SIGTSTP) == -1) || /* add signal SIGTSTP in mask*/
         (sigprocmask(SIG_SETMASK, &sigmask, 0) == -1) ) {
        perror("Error: Signal mask ");
        return 1;
    }

    sigemptyset(&act.sa_mask); /* initialize act.sa_mask */
    sigaddset(&act.sa_mask, SIGSEGV); /* add signal SIGSEGV in mask */
    act.sa_handler = on_sigint; /* setting handler */
    act.sa_flags = 0; /* setting flags */

    if ( sigaction(SIGINT, &act, 0) == -1 ) { /* catch signal SIGINT */
        perror("Error: Sigaction failed ");
        return 2;
    }

    if ( (retcode = sigsetjmp(ctx, 1)) != 0 ) {
        printf("Context is set via sigsetjmp, retcode = %d.\n", retcode);
        return 0;
    }

    printf("First call of sigsetjmp. Now pause, waits Ctrl + c.\n");
    pause(); /* waits signal */

    return 0;
}

