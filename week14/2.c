#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

#define NUM_THREADS 100
#define WORK_PER_THREAD 1000

struct Item {
    _Atomic(struct Item *) next;
    long long value;
};

_Atomic(struct Item *) head = NULL;
_Atomic(struct Item *) tail = NULL;

void *thread_func(size_t in) {
    for (int i = in * WORK_PER_THREAD; i < in * WORK_PER_THREAD + WORK_PER_THREAD; i++) {

        struct Item *new_node = malloc(sizeof(*new_node));
        new_node->value = i;
        atomic_init(&new_node->next, NULL);

        struct Item *expected_null = NULL;
        if (atomic_compare_exchange_strong(&tail, &expected_null, new_node)) {
            head = new_node;
            sched_yield();
            continue;
        }

        while (1) {

            struct Item *tail_old = atomic_load(&tail);
            struct Item *next = atomic_load(&tail_old->next);
            if (next == NULL) {
                if (atomic_compare_exchange_weak(&tail->next, &next, new_node)) {
                    sched_yield();
                    break;
                }

            } else {
                atomic_compare_exchange_strong(&tail, &tail_old, tail_old->next);
            }
        }
    }
    return NULL;
}

int main() {
    pthread_t thread_list[NUM_THREADS] = {0};

    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_list[i], NULL, (void *) (void *) thread_func, (void *) i);
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_list[i], NULL);
    }

    while (head != NULL) {
        struct Item *to_delete = head;
        printf("%lld\n", head->value);
        head = head->next;
        free(to_delete);
    }
    return 0;
}
