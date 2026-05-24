#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3
#define WORK_PER_THREAD 1000000

double array[3];
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *in) {
    size_t id = (size_t) in;
    for (int i = 0; i < WORK_PER_THREAD; i++) {
        pthread_mutex_lock(&mu);
        switch (id) {
        case 0:
            array[0] += 100;
            array[1] -= 101;
            break;
        case 1:
            array[1] += 200;
            array[2] -= 201;
            break;
        case 2:
            array[2] += 300;
            array[0] -= 301;
            break;
        }
        pthread_mutex_unlock(&mu);
    }
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
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%.10g\n", array[i]);
    }
    return 0;
}
