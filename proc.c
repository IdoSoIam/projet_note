#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <pthread.h>

int continuer = 1;

char* nomSignaux [] = {
                        "SIGHUP",     // 01
                        "SIGINT",     // 02
                        "SIGQUIT",    // 03
                        "SIGILL",     // 04
                        "SIGTRAP",    // 05
                        "SIGABRT",    // 06
                        "SIGBUS",     // 07
                        "SIGFPE",     // 08
                        "SIGKILL",    // 09
                        "SIGUSR1",    // 10
                        "SIGSEGV",    // 11
                        "SIGUSR2",    // 12
                        "SIGPIPE",    // 13
                        "SIGALRM",    // 14
                        "SIGTERM",    // 15
                        "SIGSTKFLT",  // 16
                        "SIGCHLD",    // 17
                        "SIGCONT",    // 18
                        "SIGSTOP",    // 19
                        "SIGTSTP",    // 20
                        "SIGTTIN",    // 21
                        "SIGTTOU",    // 22
                        "SIGURG",     // 23
                        "SIGXCPU",    // 24
                        "SIGXFSZ",    // 25
                        "SIGVTALRM",  // 26
                        "SIGPROF",    // 27
                        "SIGWINCH",   // 28
                        "SIGPOLL",    // 29
                        "SIGPWR",     // 30
                        "SIGSYS",     // 31
                        "SIGUNUSED"   // 32
                      };


int main (int argc, char** argv)
 {
  

  return 0;
 }