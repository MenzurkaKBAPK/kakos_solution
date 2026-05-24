#include <bits/types/sigset_t.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

volatile _Atomic int64_t accumulated = 0;
void handle_timer(int s) { accumulated += 1000; }

int main() {

    struct sigaction config = {.sa_handler = handle_timer, .sa_flags = SA_RESTART};
    sigaction(SIGALRM, &config, NULL);

    int64_t current = 0;
    int64_t total = 0;
    struct itimerval timer_settings;

    timer_settings.it_value.tv_sec = 0;
    timer_settings.it_value.tv_usec = 100 * 1000;

    timer_settings.it_interval.tv_sec = 0;
    timer_settings.it_interval.tv_usec = 100 * 1000;
    if (setitimer(ITIMER_REAL, &timer_settings, NULL) == -1) {
        return 1;
    }

    while (scanf("%ld", &current) == 1) {

        if (setitimer(ITIMER_REAL, &timer_settings, NULL) == -1) {
            return 1;
        }
        total += accumulated + current;
    }

    printf("%ld\n", total + accumulated);
    fflush(stdout);

    return 0;
}
