/* Example    : pr3-3
 * Description: Example for a 'cp' command.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define USAGE "usage: %s source_file target_file\n"

void error(char mess[]);

int main(int argc, char **argv) {
    int rd_fd;
    int wrt_fd;
    int n;
    char buff[BUFSIZ];

    /* arguments can't be less than 3 */
    if ( argc != 3 ) {
        char msg_buf[50];

        sprintf(msg_buf, USAGE, argv[0]);
        error(msg_buf);
        return 1;
    }

    /* open source file */
    if ( (rd_fd = open(argv[1],0)) == -1 ) {
        error("Error: Can't open source file!\n");
        _exit(2);
    }

    /* create target file */
    if ( (wrt_fd = creat(argv[2], 0644)) == -1 ) {
        error("Error: Can't create target file!\n");
        _exit(3);
    }

    /* copying data */
    while ( (n = read(rd_fd, buff, BUFSIZ)) > 0 ) {
        if ( write(wrt_fd, buff, n) != n ) {
            error("Error: Write error!\n");
            _exit(4);
        }
    }

    close(rd_fd);
    close(wrt_fd);

    return 0;
}

void error(char mess[]) {
    write(2, mess, strlen(mess));
}

