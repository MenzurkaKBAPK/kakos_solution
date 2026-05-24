#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MATRIX_SIZE M * N * sizeof(int64_t)

pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_id;
    int line_length;
    int64_t *read_ptr;
    int64_t *write_ptr;
} task;

void *thread_func(void *in) {
    task *task = in;

    for (int offset = 1; offset < task->line_length - 1; offset++) {
        task->write_ptr[offset] = task->read_ptr[offset + task->line_length] +
                                  task->read_ptr[offset - task->line_length] + task->read_ptr[offset + 1] +
                                  task->read_ptr[offset - 1];
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    char *file_path = argv[3];

    pthread_t *thread_list = malloc(sizeof(pthread_t) * (M - 2));
    task *task_list = malloc(sizeof(task) * (M - 2));

    int fd = open(file_path, O_RDWR);

    int64_t *matrix = mmap(NULL, M * N * sizeof(int64_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int64_t *matrix_copy = malloc(M * MATRIX_SIZE);
    memcpy(matrix_copy, matrix, MATRIX_SIZE);

    for (int id = 0; id < M - 2; id++) {
        task_list[id].thread_id = id;
        task_list[id].line_length = N;
        task_list[id].read_ptr = &matrix_copy[(id + 1) * N];
        task_list[id].write_ptr = &matrix[(id + 1) * N];
        pthread_create(&thread_list[id], NULL, thread_func, &task_list[id]);
    }

    for (int id = 0; id < M - 2; id++) {
        pthread_join(thread_list[id], NULL);
    }

    free(thread_list);
    free(task_list);
    free(matrix_copy);

    close(fd);

    return 0;
}
