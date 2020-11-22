/* Example    : pr3-1
 * Description: Process reader.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUF_SZ 9

int main() {
    int fdesc  = -1;
    int nbytes = 0;
    int nread  = 0;
    int i;
    char buf[BUF_SZ];

    if ( (fdesc = open("test", O_RDONLY)) == -1) { /* open test */
        perror("Error: Open failed ");
        return 1;
    }

    while ( (nbytes = read(fdesc, buf, BUF_SZ - 1)) > 0) { /* read test */
        nread++;
        buf[BUF_SZ - 1] = '\0';
        printf("Read %d: %s (%d bytes).\n", nread, buf, nbytes);

        if ( (nbytes == 0) && (nread > 3) )
            _exit(0);

        for ( i = 0; i < 8; i++ )
            buf[i] = ' ';

        if ( nbytes == -1 ) {
            perror("Error: Read failed ");
            _exit(0);
        }
    }

    return 0;
}

