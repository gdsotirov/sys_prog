/* Example    : pr6-11
 * Description: File map example.
 * Edited by  : George D. Sotirov, astornom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    char ch = '\0';
    int fildes, i;
    caddr_t memaddr; /* address */

    /* crete new file for read/write */
    if ( (fildes = open("test", O_CREAT | O_EXCL | O_RDWR, 0666)) == -1 ) {
        perror("Error: Open failed ");
        exit(1);
    }

    for ( i = 0; i < 512; i++ )
        write(fildes, &ch, 1); /* initialize file */

    /* make file into memory */
    if ( (memaddr = mmap(0, 512, PROT_READ | PROT_WRITE, MAP_SHARED, fildes, 0)) == MAP_FAILED ) {
        perror("Error: Mmap failed ");
        exit(2);
    }

    /* close file */
    close(fildes);

    /* read the terminal into memory */
    fgets(memaddr, 80, stdin);

    /* unmap mapped memory */
    munmap(memaddr, 512);

    exit(0);
}

