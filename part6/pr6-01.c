/* Example    : pr6-1
 * Description: Messages IPC example (send message).
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"

struct msgbuf message = {MSG_TYPE, "Hello. I am sender!\0"};

int main() {
    int qdes; /* queue descriptor with key 10 */

    /* open queue */
    if ( (qdes = msgget(MSG_KEY, IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
        perror("Error: Msgget failed ");
        return 1;
    }

    /* send message */
    if ( msgsnd(qdes, &message, sizeof(message), IPC_NOWAIT) == -1 ) {
        perror("Error: Msgsnd failed ");
        return 2;
    }

    return 0;
}

