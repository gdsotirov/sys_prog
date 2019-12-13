/* Example
 * Description: Special file example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SZ 8192

int main() {
    char buf1[BUF_SZ];
    char buf2[BUF_SZ];
    int fd1;
    int fd2;
    int ofs;
    int diff = 0; /* shows if there a difference between files */

    fd1 = open("/dev/dsk3", O_RDONLY); /* open special block device file */
    fd2 = open("/dev/rdsk3", O_RDONLY); /* open special char device file */

    lseek(fd1, (off_t)0, SEEK_END); /* seeking to the end of file 1 */
    lseek(fd2, (off_t)0, SEEK_END); /* seeking to the end of file 2 */

    read(fd1, buf1, sizeof(buf1)); /* read file 1 */
    read(fd2, buf2, sizeof(buf2)); /* read file 2 */

    for ( ofs = 0; ofs < sizeof(buf1); ofs++ ) { /* compare the two files */
        if ( buf1[ofs] != buf2[ofs] ) {
            printf("There is a difference at offset %d.\n", ofs);
            diff = 1;
        }
    }

    if ( diff == 0 )
        printf("The data match.\n");

    return 0;
}

