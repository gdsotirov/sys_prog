/* Example    : pr6-3
 * Description: Messages IPC example (receive message).
 * Edited by  : George D. Sotirov, astornom@dir.bg
 */

#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"

struct msgbuf message;

int main() {
    int qdes; /* queue descriptor with key 10 */

    if ( (qdes = msgget(MSG_KEY, 0)) == -1 ) { /* open queue */
        perror("Error: Msgget failed ");
        return 1;
    }

    /* check for message */
    if ( msgrcv(qdes, &message, sizeof(message), MSG_TYPE, IPC_NOWAIT | MSG_NOERROR) == -1 ) {
        if ( errno == E2BIG ) {
            printf("There is message in the queue, but it is to big.\n");
            return 2;
        }

        perror("Error: Msgrcv failed ");
        return 3;
    }

    printf("Received message:\n");
    printf("Type: %ld\n", message.mtype);
    printf("Data: %s\n", message.mtext);

    return 0;
}

