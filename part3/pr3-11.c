/* Example    : pr3-11
 * Description: File access example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int filetest(char *path, int *fd) {
    if  ( access(path, F_OK) == 0 ) {
        *fd = open(path, O_RDONLY); /* open file */
        return 0;
    }
    else {
        *fd = open(path, O_WRONLY | O_CREAT, 0644);
        return 1;
    }
}

int main(int argc, char **argv) {
    int *fdesc = NULL; /* file descriptor */

    if ( argc < 2 ) {
        printf("usage: %s filename\n", argv[0]);
        return 1;
    }

    if ( filetest(argv[1], fdesc) == 0 )
        printf("File '%s' exists!\n", argv[1]);
    else {
        printf("File '%s' don't exists! It was created.\n", argv[1]);
        return 2;
    }

    return 0;
}

