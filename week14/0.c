#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stddef.h>

#define NUM_THREADS 10

size_t current_count = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *in) {
    size_t id = (size_t) in;

    pthread_mutex_lock(&mu);

    while (current_count != id) {
        pthread_cond_wait(&cond, &mu);
    }

    printf("%zu\n", current_count);
    fflush(stdout);
    current_count += 1;
    pthread_mutex_unlock(&mu);
    pthread_cond_broadcast(&cond);
    return NULL;
}

int main() {
    pthread_t thread_list[NUM_THREADS] = {0};

    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_list[i], NULL, thread_func, (void *) i);
    }
    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_list[i], NULL);
    }
    return 0;
}
