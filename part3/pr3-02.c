/* Example   : pr3-2
 * Descripton: Process writer.
 * Edited by : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define MODE 0744 /* -rwxr--r-- */

int main() {
    int fdesc;
    char buf[11] = ("0123456789");

    fdesc = open("test", O_CREAT | O_WRONLY | O_TRUNC, MODE);
    write(fdesc, buf, sizeof(buf)); /* write in test */
    printf("Write #1 in test: %s\n", buf);
    write(fdesc, "0123456789", 10); /* write in test */
    printf("Write #2 in test: 0123456789\n");
    write(fdesc, buf, 10); /* write in test */
    printf("Write #3 in test: %s\n", buf);

    return 0;
}

