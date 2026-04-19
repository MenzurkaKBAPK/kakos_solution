#include "unistd.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>

void recieve_data(int socket_fd) {
    shutdown(socket_fd, SHUT_WR);
    char buff[2048];
    int n_bytes = 0;
    while ((n_bytes = read(socket_fd, &buff, sizeof(buff) - 1)) > 0) {
        write(STDOUT_FILENO, &buff, n_bytes);
    }
}

#define MAX_RETRY 5
int main(int argc, char **argv) {

    if (argc != 2) {
        return 1;
    }

    char *addr = argv[1];
    struct sockaddr_un config = {.sun_family = AF_UNIX};
    strcpy(config.sun_path, addr);

    int attempts_left = MAX_RETRY;
    for (; attempts_left != 0; attempts_left--) {
        int socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

        if (socket_fd == -1) {
            return 1;
        }

        if (connect(socket_fd, (struct sockaddr *) &config, sizeof(config)) == 0) {
            recieve_data(socket_fd);
            close(socket_fd);
            return 0;
        }
        close(socket_fd);
        usleep(50 * 1000);
    }
    printf("No connection\n");

    return 0;
}
