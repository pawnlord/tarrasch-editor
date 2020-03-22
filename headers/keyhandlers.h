#include  <stdio.h>
#include  <signal.h>
#include  <stdlib.h>
static volatile int running = 1;

void  INThandler(int sig) {
	running = 0;
}

void start_handler(void (*handler)(int sig), int SIG) {
    signal(SIG, handler);
}