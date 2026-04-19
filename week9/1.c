#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MAX_SIGNAL_COUNT 5

volatile sig_atomic_t sig_counter = 0;

void set_mode(int s) {
    char buff[3];
    snprintf(buff, sizeof(buff), "%1d\n", sig_counter);
    write(STDOUT_FILENO, buff, sizeof(buff) - 1);
    sig_counter++;
    if (sig_counter == MAX_SIGNAL_COUNT) {
        exit(0);
    }
}

int main() {
    struct sigaction config = {.sa_handler = set_mode, .sa_flags = SA_RESTART};

    sigaction(SIGHUP, &config, NULL);

    pid_t my_pid = getpid();

    printf("%d\n", my_pid);
    fflush(stdout);

    while (1) {
        pause();
    }
    return 1;
}
