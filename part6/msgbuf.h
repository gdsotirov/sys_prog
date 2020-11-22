/* File       : msgbuf.h
 * Description: Constant msgbuf structure definitions.
 * Written by : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#define MSG_KEY         88
#define MSG_TYPE        8
#define MSGBUF_LEN      256

struct msgbuf {
    long mtype;
    char mtext[MSGBUF_LEN];
};

/* Message structure */
struct msgstruct {
    int uid;    /* requested service */
    int pid;    /* requester process id */
};

