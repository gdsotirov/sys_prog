/* Example    : pr2-6
 * Description: Demonstrates nice system call.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE "usage: %s [-incr|-n incr] command\n"

int main(int argc, char **argv) {
    int incr;    /* priority increment */
    int argnumb; /* command arguments count */

    if ( argc < 2 ) { /* error in arguments */
        fprintf(stderr, USAGE, argv[0]);
        exit(1);
    }

    if ( argv[1][0] != '-' ) { /* default */
        incr = 10;
        argnumb = 1;
    }
    else if ( argv[1][1] == 'n' ) { /* superuser (root) */
        if ( getuid() != 0 ) {
            fprintf(stderr, "Error: You must be superuser (root)!\n");
            exit(1);
        }

        incr = -(atoi(argv[2]));
        argnumb = 3;
    }
    else { /* normal user */
        incr = atoi(&argv[1][1]);
        argnumb = 2;
    }

    if ( argc != argnumb + 1 ) { /* check for error in arguments */
        fprintf(stderr, USAGE, argv[0]);
        exit(2);
    }

    nice(incr); /* priority modification */

    execvp(argv[argnumb], &argv[argnumb]); /* execution of command */
    perror("Error: Execvp failed ");

    return 0;
}

