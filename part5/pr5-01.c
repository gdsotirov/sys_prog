/* Example    : pr5-1
 * Description: Demonstration of signal system call.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

FILE *fildes, *fopen();

void on_intr(int catch_sig) { /* signal handler */
    char go_on;

    printf("Interrupt (signal %d is caught. Go on? [y] ", catch_sig);
    scanf("%c", &go_on); /* let user choose how process to continue */

    if ( go_on == 'y' ) {
        signal(SIGINT, on_intr);
        return; /* return to 'main' */
    }

    exit(1); /* the process ends */
}

int main(int argc, char **argv) {
    char str[80];

    if ( argc < 2 ) {
        printf("usage: %s file_name\n", argv[0]);
        return 0;
    }

    if ( (signal(SIGINT, on_intr)) == SIG_ERR ) { /* catch signal */
        perror("Error: Signal failed ");
        return 1;
    }

    if ( (fildes = fopen(argv[1], "r")) == NULL ) { /* open file in argv[1] */
        perror("Error: Fopen failed ");
        exit(2);
    }

    /* print out the file in argv[1] */
    while ( (fgets(str, 80, fildes)) != NULL ) {
        fputs(str, stdout);
        sleep(1);
    }
    fputs("\n", stdout);

    fclose(fildes); /* close file */

    return 0;
}

