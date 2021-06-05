// handle requst
#ifndef X_SERVER_REQ_H
#define X_SERVER_REQ_H

#include <sys/socket.h>

#include "../conf/conf.h"

#define READ_BUFFER_SIZE 65536
#define URI_BUFFER_SIZE 256
#define BODY_BUFFER_SIZE 35536

#define NOT_ALLOW_METHOD -1
#define METHOD_GET 1
#define METHOD_POST 2
#define METHOD_DELETE 3
typedef int http_method_t;

#define CONN_CLOSE 1
#define CONN_KA 2
typedef int http_conn_status_t;

extern conf_t conf;

void init_router_handle();

void* http_req_handle(int connfd, struct sockaddr* cli_addr,
                      socklen_t* cli_socklen);

#endif