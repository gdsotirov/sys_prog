/* Example    : pr5-2
 * Description: Demonstrates ioctl system call and termios interface.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

struct termios old_tty; /* old attributes */
void on_intr(int catch_sig); /* signal handler */

int main() {
    struct termios new_tty; /* new attributes */
    int n;
    char buf[10];

    sigset(SIGINT, on_intr); /* signal catch */

    if ( tcgetattr(0, &old_tty) == -1 ) { /* read terminal attributes */
        perror("Error: Tcgetattr failed ");
        exit(1);
    }

    new_tty = old_tty;
    new_tty.c_lflag = ~ICANON; /* turn off canonic mode */
    new_tty.c_cc[VMIN] = 8; /* read with 8 chars */

    if ( tcsetattr(0, TCSANOW, &new_tty) == -1 ) { /* setting new attributes */
        perror("Error: Tcsetattr failed ");
        exit(2);
    }

    while ( (n = read(0, buf, sizeof(buf))) > 0 ) { /* read terminal */
        buf[n] = 0;
        printf("*** %d characters read: %s\n", n, buf);
    }

    return 0;
}

void on_intr(int catch_sig) {
    tcsetattr(0, TCSADRAIN, &old_tty); /* back to old attributes */
    exit(0);
}

