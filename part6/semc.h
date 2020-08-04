/* File       : semun.h
 * Description: Common definitions for semaphore examples.
 * Written by : George D. Sotirov, astronom@dir.bg
 */

#include <sys/sem.h>

union semun_u {
    int             val;        /* semaphore value                      */
    struct semid_ds *buf;       /* control data for semaphor array      */
    unsigned short  *array;     /* values of all sempahors in array     */
};

