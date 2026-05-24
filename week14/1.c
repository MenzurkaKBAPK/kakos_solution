#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <stddef.h>

void *thread_func(void *in) {
    int my_number = 0;

    if (scanf("%d", &my_number) == 1) {
        pthread_t new_thread = {0};

        pthread_create(&new_thread, NULL, (void *) (void *) thread_func, NULL);
        pthread_join(new_thread, NULL);
        printf("%d\n", my_number);
        fflush(stdin);
    }
    return NULL;
}

int main() {
    pthread_t new_thread = {0};

    pthread_create(&new_thread, NULL, (void *) (void *) thread_func, NULL);
    pthread_join(new_thread, NULL);

    return 0;
}

// #include <unistd.h>
// #include <stdio.h>
// #include <sys/wait.h>

// int main(int argc, char **argv) {
//     int number = 0;
//     pid_t pid = 0;
//     int my_number = 0;
//     while (scanf("%d", &number) == 1) {
//         if ((pid = fork()) < 0) {
//             return 1;
//         }
//         if (pid != 0) {
//             my_number = number;
//             waitpid(pid, NULL, 0);
//             printf("%d\n", my_number);

//             break;
//         }
//     }

//     return 0;
// }
