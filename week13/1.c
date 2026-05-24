#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

struct Buffer;

int buffer_put(struct Buffer *b, const void *data, size_t dsize);
size_t buffer_get(struct Buffer *b, void *outdata, size_t size);
void buffer_reset(struct Buffer *b);
void buffer_free(struct Buffer *b);
const void *buffer_data(const struct Buffer *b);
size_t buffer_size(const struct Buffer *b);
void buffer_advance(struct Buffer *b, size_t size);

#define UASSERT(expr)                                                                                                  \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
            fprintf(stderr, "%s:%d: assertion failed: %s\n", __FILE__, __LINE__, #expr);                               \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

int ABOBA() {
    char *aboba = (char *) 0xAB0BA;
    return *aboba; // ABOBA
}

void test_0() {
    struct Buffer b = {};

    UASSERT(buffer_size(&b) == 0);
    buffer_free(&b);
}

void test_1() {
    struct Buffer b = {};
    char data[CHAR_MAX];
    for (int i = 0; i < sizeof(data); i++) {
        data[i] = (char) i;
    }
    UASSERT(buffer_put(&b, data, sizeof(data)) == 0);
    UASSERT(buffer_size(&b) == sizeof(data));
    for (int offset = 0; offset < sizeof(data); offset++) {
        UASSERT(data[offset] == ((char *) buffer_data(&b))[offset]);
    }
    buffer_free(&b);
}

void test_2() {
    struct Buffer b = {};
    char data[CHAR_MAX];
    char outdata[CHAR_MAX];
    for (int i = 0; i < sizeof(data); i++) {
        data[i] = (char) i;
    }
    UASSERT(buffer_put(&b, data, sizeof(data)) == 0);
    UASSERT(buffer_size(&b) == sizeof(data));
    UASSERT(buffer_get(&b, &outdata, sizeof(outdata)) == sizeof(data));
    for (int offset = 0; offset < sizeof(data); offset++) {
        UASSERT(data[offset] == outdata[offset]);
    }
    buffer_free(&b);
}

void test_3() {
    struct Buffer b = {};
    char data[CHAR_MAX];
    char outdata[CHAR_MAX];
    for (int i = 0; i < sizeof(data); i++) {
        data[i] = (char) i;
    }
    UASSERT(buffer_put(&b, data, sizeof(data)) == 0);
    UASSERT(buffer_size(&b) == sizeof(data));
    UASSERT(buffer_get(&b, &outdata, sizeof(outdata)) == sizeof(data));
    for (int offset = 0; offset < sizeof(data); offset++) {
        UASSERT(data[offset] == outdata[offset]);
    }
    buffer_free(&b);
}
void test_4() {
    struct Buffer b = {};
    char data[CHAR_MAX];
    char outdata[CHAR_MAX];
    for (int i = 0; i < sizeof(data); i++) {
        data[i] = (char) i;
    }
    for (int i = 0; i < sizeof(data); i++) {
        UASSERT(buffer_put(&b, &data[i], 1) == 0);
    }
    UASSERT(buffer_size(&b) == sizeof(data));

    for (int i = 0; i < sizeof(data); i++) {
        UASSERT(buffer_get(&b, &outdata[i], 1) == 1);
    }
    for (int offset = 0; offset < sizeof(data); offset++) {
        UASSERT(data[offset] == outdata[offset]);
    }
    buffer_free(&b);
}

int main() {
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle) {
        abort();
    }

    int i = 0;
    while (1) {
        char name[128];
        snprintf(name, sizeof(name), "test_%d", i);
        void (*func)() = dlsym(handle, name);
        if (!func) {
            break;
        }
        func();
        fflush(stdout);
        ++i;
    }
    if (i) {
        puts("1");
    }
}
