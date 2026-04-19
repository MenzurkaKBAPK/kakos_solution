#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <signal.h>

#define MAX_SIGNAL_COUNT 3

volatile sig_atomic_t sig_counter = 0;
volatile sig_atomic_t last_prime = 0;

void handle_term(int s) { exit(0); }

void handle_int(int s) {
    if (sig_counter == MAX_SIGNAL_COUNT) {
        exit(0);
    }
    sig_counter++;

    int length = snprintf(NULL, 0, "%d\n", last_prime);
    char *str = malloc(length + 1); // так по идее не надо делать
    snprintf(str, length + 1, "%d\n", last_prime);

    write(STDOUT_FILENO, str, length);
    free(str);
}

int is_prime(int num) {
    if (num < 2) {
        return 0;
    }

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int low = 0;
    int high = 0;

    struct sigaction config_sigterm = {.sa_handler = handle_term, .sa_flags = SA_RESTART};
    struct sigaction config_sigint = {.sa_handler = handle_int, .sa_flags = SA_RESTART};

    sigaction(SIGTERM, &config_sigterm, NULL);
    sigaction(SIGINT, &config_sigint, NULL);

    if (scanf("%d %d", &low, &high) != 2) {
        return 1;
    }

    pid_t my_pid = getpid();
    printf("%d\n", my_pid);
    fflush(stdout);

    for (int i = low; i < high; i++) {
        if (is_prime(i)) {
            last_prime = i;
        }
    }
    printf("%d\n", -1);
    fflush(stdout);
    return 0;
}
