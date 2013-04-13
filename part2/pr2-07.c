/* Example    : pr2-7
 * Description: Demonstrates brk and sbrk system calls.
 * Edited by  : George D. Sotirov, astronom@dir.bg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char *last_addr;
int i;
int call_numb = 0;

void sig_handler(int sig_numb) {
    char ch;
    /*int clr;*/

    call_numb++;

    printf("Sig_handler(signal %d) - %dth call on address %p\n", sig_numb, call_numb, last_addr);
    printf("Exit [y/n]? ");

    switch ( ch = getchar() ) {
        case 'y' :  exit(0);
        default  :  printf("%c\n", ch); break;
    }

    /*clr =*/ getchar();
    sbrk(512);
}

int main(void) {
    sigset(SIGSEGV, sig_handler);
    last_addr = (char *)sbrk(0);
    printf("Original break value: %p\n", last_addr);
    for ( ;; ) {
        i = i + 1;
        last_addr += 1;
        *last_addr = 'a';
        printf("Last address = %p, value = %c, i = %d\n", last_addr, *last_addr, i);
    }

    return 1;
}

