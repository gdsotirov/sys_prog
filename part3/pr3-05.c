/* Example    : pr3-5
 * Description: Hard links example.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    if ( (argc != 3) || !strcmp(argv[1],argv[2]) ) {
        printf("usage: %s source_file target_file\n", argv[0]);
        return -1;
    }

    if ( link(argv[1], argv[2]) == -1 ) {
        perror("Error: Link failed ");
        return -1;
    }

    if ( unlink(argv[1]) == -1 ) {
        perror("Error: Unlink failed ");
        return -1;
    }

    return 0;
}

