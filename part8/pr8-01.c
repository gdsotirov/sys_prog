/* Example    : pr8-1
 * Description: Multithreaded example.
 * Author     : 2000, Lylan Nikolov
 * Edited by  : 2003, George D. Sotirov, astronom@dir.bg
 */

#include "thr_def.h"

#define CHAIRS 3        /* chairs count                                 */

sema_t mutex     = 1;   /* binary semaphore                             */
sema_t barber    = 0;   /* binary semaphore - barber waiting for client */
sema_t customers = 0;   /* semaphore counter for the clients            */
thread_t id0, id1, id2; /* threads identificators                       */
int waiting = 0;        /* count of waiting clients                     */

void service(void) {            /* Barber serving client                */
    puts("The barber is cutting customers hair.\n");
    thr_delay(100);             /* simulate barbers work                */
}

void get_service(thread_t id) { /* client got served                    */
    thr_delay(120);             /* simulate service                     */
    printf("Customer %d got his hair cut.\n", id);
}

void Barber(void) {             /* barber with thread id id0            */
    while ( 1 ) {
        sema_pc(&customers);    /* barber goes to sleep, if no customer */
        sema_pb(&mutex);        /* else, get 'waiting' and              */
        waiting--;              /* decrease clients count               */
        sema_vb(&barber);       /* barber ready for work                */
        sema_vb(&mutex);        /* release 'waiting'                    */
        service();              /* serve client                         */
        thr_yield();            /* refuse the processor                 */
    }
}

void Customer1(void) {          /* client - thread with id1             */
    puts("Customer 1 enters the shop.\n");
    sema_pb(&mutex);            /* got 'waiting'                        */

    if ( waiting < CHAIRS ) {   /* if there is no free chair            */
        waiting++;              /* increase waitings count              */
        puts("Customer 1 sits in the waiting room.");
        sema_vc(&customers);    /* wake up the barber                   */
        sema_vb(&mutex);        /* release 'waiting'                    */
        sema_pb(&barber);       /* block if the barber is busy          */
        get_service(id1);       /* gets service                         */
    }
    else {
        printf("Customer %d leaves the shop.\n", id1);
        sema_vb(&mutex);        /* leave shop is there is no free chair */
    }

    thr_delete(id1);            /* destroy thread                       */
}

void Customer2(void) {          /* client - thread with id2             */
    puts("Customer 2 enters the shop.\n");
    sema_pb(&mutex);            /* got 'waiting'                        */

    if ( waiting < CHAIRS ) {   /* if there is no free chair            */
        waiting++;              /* increase waitings count              */
        puts("Customer 2 sits in the waiting room.");
        sema_vc(&customers);    /* wake up the barber                   */
        sema_vb(&mutex);        /* release 'waiting'                    */
        sema_pb(&barber);       /* block if the barber is busy          */
        get_service(id2);       /* gets service                         */
    }
    else {
        printf("Customer %d leaves the shop.\n", id1);
        sema_vb(&mutex);        /* leave shop is there is no free chair */
    }

    thr_delete(id2);            /* destroy thread                       */
}

int main(void) {
    size_t STK = 3000; /* stack size */

    /* begin multithreading mode */
    parbegin();

    thr_create(Barber,    STK, &id0);
    thr_create(Customer1, STK, &id1);
    thr_create(Customer2, STK, &id2);

    /* start multithreading work */
    parend();

    return 0;
}

