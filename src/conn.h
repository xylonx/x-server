#ifndef X_SERVER_CONN_H
#define X_SERVER_CONN_H

#include <sys/socket.h>

#define LISTEN_Q 5

#define CLI_INFO_BUF_SIZE 22

// epoll related
#define MAX_FD 1024
#define MAX_EVNET 256
#define EPOLL_WAIL_DURATION 10
#define EPOLL_CONN_ET

// http related
#define VERB_CLOSE_CONN 1

// the value of it is the server action.
// e.g. close connection.
typedef int serv_verb_t;

// fd is the connection file descriptor
// client_addr and cli_addrlen contain client info
typedef void* (*req_handle)(int, struct sockaddr*, socklen_t*);

// return listen file descriptor
int start_listen(char* addr, int port);
void* handle_conn_epoll(int listenfd, req_handle in_handle,
                        req_handle err_handle);

#endif