/* Example    : pr6-4
 * Description: Message queues control example.
 * Edited by  : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgbuf.h"

int main(int argc, char *argv[]) {
    int qdes;
    struct msqid_ds mbuf;

    if ( (qdes = msgget(MSG_KEY, 0)) == -1 ) {
        perror("Error: Msgget failed ");
        return 1;
    }

    if ( msgctl(qdes, IPC_STAT, &mbuf) == -1 ) { /* read queue status */
        perror("Error: Msgctl failed ");
        return 2;
    }

    mbuf.msg_perm.mode = 0640; /* new permissions mode */

    if ( msgctl(qdes, IPC_SET, &mbuf) == -1 ) { /* set new status */
        perror("Error: Msgctl failed ");
        return 3;
    }

    /* printout message queue information */
    printf("Queue %d\n", qdes);
    printf("-----------\n");
    printf("Permissions>\n");
    printf(" Effective uid of owner      : %d\n", mbuf.msg_perm.uid);
    printf(" Effective gid of owner      : %d\n", mbuf.msg_perm.gid);
    printf(" Effective uid of creator    : %d\n", mbuf.msg_perm.cuid);
    printf(" Effective gid of creator    : %d\n", mbuf.msg_perm.cgid);
    printf(" Access mode for own,grp,oth : %d\n", mbuf.msg_perm.mode);
#ifdef _CYGWIN_IPC_H
    printf(" Sequence counter            : %d\n", mbuf.msg_perm.seq);
#elif defined(__DARWIN_UNIX03) /* Apple */
    printf(" Sequence counter            : %d\n", mbuf.msg_perm._seq);
#else
    printf(" Sequence counter            : %d\n", mbuf.msg_perm.__seq);
#endif
#if defined(__sun__) || defined (__solaris__)
    printf(" Key                         : %d\n", mbuf.msg_perm.key);
#elif defined _CYGWIN_IPC_H
    printf(" Key                         : %lld\n", mbuf.msg_perm.key);
#elif defined(__DARWIN_UNIX03) /* Apple */
    printf(" Key                         : %d\n", mbuf.msg_perm._key);
#else
    printf(" Key                         : %d\n", mbuf.msg_perm.__key);
#endif
    printf("Counters>\n");
#ifdef __CYGWIN__
    printf(" Total byte count            : %ui\n", mbuf.msg_cbytes);
    printf(" Total message count         : %ui\n", mbuf.msg_qnum);
#else
    printf(" Total byte count            : %lu\n", mbuf.msg_cbytes);
    printf(" Total message count         : %lu\n", mbuf.msg_qnum);
#endif
    printf("Limits>\n");
#ifdef __CYGWIN__
    printf(" Max byte count in msgs      : %ui\n", mbuf.msg_qbytes);
#else
    printf(" Max byte count in msgs      : %lu\n", mbuf.msg_qbytes);
#endif
    printf("Process info>\n");
    printf(" Proc last send message (id) : %d\n" ,mbuf.msg_lspid);
    printf(" Proc last read message (id) : %d\n", mbuf.msg_lrpid);
    printf("Timers>\n");
    printf(" Time of send on last mesg   : %s", ctime(&mbuf.msg_stime));
    printf(" Time of read on last mesg   : %s", ctime(&mbuf.msg_rtime));
    printf(" Time of last param change   : %s", ctime(&mbuf.msg_ctime));

    if ( msgctl(qdes, IPC_RMID, 0) == -1 ) { /* remove queue */
        perror("Error: Msgctl failed ");
        return 4;
    }

    return 0;
}

