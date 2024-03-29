#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "conf/conf.h"
#include "http/conn.h"
#include "http/req.h"
#include "http/resp.h"

conf_t conf;

void* echo_handle(int fd, struct sockaddr* cli_addr, socklen_t* cli_addrlen);

int main(int argc, char const* argv[]) {
    init_conf(&conf);
    init_router_handle();

    int listenfd = start_listen(conf->server.bind_addr, conf->server.bind_port);

    handle_conn_epoll(listenfd, http_req_handle, NULL);
    // for test. tcp echo server
    // handle_conn_epoll(listenfd, echo_handle, NULL);

    return 0;
}

// #define BUFFER_SIZE 1024
// void* echo_handle(int connfd, struct sockaddr* addr, socklen_t* addrlen) {
//     printf("[Test] receive file descriptor: %d\n", connfd);
//     char buffer[BUFFER_SIZE];
//     bzero(buffer, BUFFER_SIZE);
//     read(connfd, buffer, BUFFER_SIZE);
//     printf("read bytes from %d:\n%s\n", connfd, buffer);
//     write(connfd, buffer, strlen(buffer));
//     return NULL;
// }