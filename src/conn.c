#include "conn.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>

#include "conf.h"

int start_listen() {
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // init and set servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, conf->server.bind_addr, &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(conf->server.bind_port);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        // TODO: log
        printf("[Error] Cannot bind address %s at port %d\n",
               conf->server.bind_addr, conf->server.bind_port);
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, LISTEN_Q) < 0) {
        // TODO: log
        printf("[Error] Cannot listen at %s on port %d\n",
               conf->server.bind_addr, conf->server.bind_port);
        exit(EXIT_FAILURE);
    }

    return listenfd;
}