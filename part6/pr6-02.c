/* Example    : pr6-2
 * Description: Messages IPC example (receiving message).
 * Edited by  : George D. Sotirov, astornom@dir.bg
 */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    int qdes; /* queue descriptor with key 10 */

    if ( (qdes = msgget(10, 0)) == -1 ) { /* open queue */
        perror("Error: Msgget failed ");
        return 1;
    }

    if ( msgrcv(qdes, NULL, 0, 5, IPC_NOWAIT) == -1 ) { /* receive message */
        perror("Error: Msgrcv failed ");
        return 2;
    }

    return 0;
}

