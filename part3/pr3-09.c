/* Example    : pr3-9
 * Description: Example for named pipes.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    int fd;
    int fd1;
    int pid;
    int len;
    char readbuf[BUFSIZ];
    char writebuf[BUFSIZ];

    if ( argc != 2 ) {
        printf("usage: %s file_name\n", argv[0]);
        exit(0);
    }

    mkfifo("MYFIFO", S_IFIFO | 0666); /* create fifo file */
    switch ( pid = fork() ) { /* create process reader */
        case -1 :   perror("Error: Fork failed!\n");
                    exit(1);
        case  0 :   /* child opens pipe for reading */
                    fd = open("MYFIFO", O_RDONLY);
                    while ( (len = read(fd, readbuf, sizeof(readbuf))) > 0 )
                        printf("%s", readbuf); /* print data to screen */
        default :   /* parent opens pipe for writing */
                    fd = open("MYFIFO", O_WRONLY);
                    fd1 = open(argv[1], O_RDONLY); /* parent opens file */
                    while ( (len = read(fd1, writebuf, sizeof(writebuf))) > 0 )
                        write(fd, writebuf, len);
                    close(fd1);
    }

    close(fd);

    unlink("MYFIFO");

    return 0;
}

