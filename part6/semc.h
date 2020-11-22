/* File       : semun.h
 * Description: Common definitions for semaphore examples.
 * Written by : Georgi D. Sotirov, gdsotirov@gmail.com
 */

#include <sys/sem.h>

union semun_u {
    int             val;        /* semaphore value                      */
    struct semid_ds *buf;       /* control data for semaphor array      */
    unsigned short  *array;     /* values of all sempahors in array     */
};

