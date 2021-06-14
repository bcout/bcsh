/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_signals.c
 * * * * * * * * * * * * * * * * * * *
 * Signal handlers will be implemented here
 * * * * * * * * * * * * * * * * * * *
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

extern volatile sig_atomic_t signal_handled;

void signal_handler(int signum){
    signal_handled = 1;
    signal(signum, signal_handler);
}
