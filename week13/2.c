#include <signal.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>

typedef struct {
    int client_fd;
    char *unwritten_data;
} ClientInfo;

void unblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

char *parse_port() {
    int port;

    if (scanf("%d", &port) != 1) {
        return NULL;
    }

    int port_len = snprintf(NULL, 0, "%d", port);

    char *port_str = malloc(port_len + 1);
    sprintf(port_str, "%d", port);

    return port_str;
}

int bind_nonblock(char *port) {
    struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    struct addrinfo *result = NULL;

    if (getaddrinfo("0.0.0.0", port, &hints, &result) != 0) {
        return -1;
    }

    int sfd = socket(PF_INET, SOCK_STREAM, 0);

    int val = 1;

    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    unblock(sfd);

    if (bind(sfd, result->ai_addr, result->ai_addrlen) != 0) {
        return -1;
    }

    return sfd;
}
int main() {

    int total_clients = 0;
    char *port_str = parse_port();
    if (port_str == NULL) {
        return 1;
    }
    int listening_socket = bind_nonblock(port_str);
    free(port_str);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);

    struct epoll_event event_listen = {.events = EPOLLIN};

    int epoll_fd = epoll_create1(NULL);

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listening_socket, &event_listen);

    while (1) {
    }
}
