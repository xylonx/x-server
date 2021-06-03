#define _GNU_SOURCE
#include "conn.h"

#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "conf.h"
// #include "ctrl.h"

void epoll_wait_error_handle(int errsv) {
    switch (errsv) {
        case EBADF:
            printf("[Error]: epoll_wait: the epfd is not valid\n");
            return;

        case EFAULT:
            printf(
                "[Error] epoll_wait: the events memory is not "
                "writeable.\n");
            return;

        case EINVAL:
            printf(
                "[Error]: epoll_wait: epfd is not an epoll file "
                "descriptor, or maxevents is less than or equal to "
                "zero.");
            return;
    }
}

void get_cli_info(const struct sockaddr* sockaddr, char* buf, int buf_size) {
    struct sockaddr_in* cli_addr_in = (struct sockaddr_in*)sockaddr;
    char ipv4buf[16];
    inet_ntop(AF_INET, &cli_addr_in->sin_addr, ipv4buf, 16);
    snprintf(ipv4buf, sizeof(ipv4buf), "%s:%d", ipv4buf,
             ntohs(cli_addr_in->sin_port));
}

int start_listen(char* addr, int port) {
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // init and set servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, addr, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
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

// handle connection with epoll module.
// in_handle is used to handle received tcp message. Here is http stream.
// err_handle is used to handle abnormal connection close event
void* handle_conn_epoll(int listenfd, req_handle in_handle,
                        req_handle err_handle) {
    int epollfd, awake_event_num, connfd;
    struct epoll_event init_ev, awake_ev, conn_ev, events[MAX_EVNET];
    char cli_info_buf[CLI_INFO_BUF_SIZE];

    // client info holder
    struct sockaddr cli_addr[MAX_FD];
    socklen_t cli_socklen[MAX_FD];

    // create epoll instance
    if ((epollfd = epoll_create1(0)) < 0) {
        // TODO: log
        printf("[Error] create epoll file descriptor failure.\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    // init first epoll event: tcp socket listen
    init_ev.events = EPOLLIN;
    init_ev.data.fd = listenfd;

    // add tcp socket listen event
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &init_ev) == -1) {
        // TODO: log
        printf("[Error]: epoll_ctl: add listen file descriptor failure.\n");
        exit(EXIT_FAILURE);
    }

    // handle events
    for (;;) {
        if ((awake_event_num = epoll_wait(epollfd, events, MAX_EVNET,
                                          EPOLL_WAIL_DURATION)) < 0) {
            // TODO: log
            int errsv = errno;
            epoll_wait_error_handle(errsv);
            exit(EXIT_FAILURE);
            return NULL;
        }

        for (int i = 0; i < awake_event_num; i++) {
            awake_ev = events[i];

            // handle new connection establish event
            if (awake_ev.data.fd == listenfd) {
                // init client info holder
                bzero(&cli_addr[awake_ev.data.fd], sizeof(struct sockaddr));
                bzero(&cli_socklen[awake_ev.data.fd], sizeof(socklen_t));
                bzero(cli_info_buf, CLI_INFO_BUF_SIZE);
                get_cli_info(&cli_addr[awake_ev.data.fd], cli_info_buf,
                             CLI_INFO_BUF_SIZE);

                if ((connfd = accept4(listenfd, &cli_addr[awake_ev.data.fd],
                                      &cli_socklen[awake_ev.data.fd],
                                      SOCK_NONBLOCK)) == -1) {
                    // TODO: log
                    printf(
                        "[Warning] accept connection failure. remote client "
                        "info: %s\n",
                        cli_info_buf);

                    // accept error, should not continue running. Interrupt this
                    // accept.
                    continue;
                };

                printf("[Info] client info: %s\n", cli_info_buf);

                bzero(&conn_ev, sizeof(conn_ev));

// set epoll trigger type
#ifdef EPOLL_CONN_ET
                conn_ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
#else
                conn_ev.events = EPOLLIN | EPOLLRDHUP;
#endif
                conn_ev.data.fd = connfd;

                // add connection to epoll event list
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &conn_ev) == -1) {
                    // TODO: log
                    printf("[Warning] add connection to epoll failed\n");
                    continue;
                }
            }

            // handle tcp connection
            else {
                // handle abnormal closed connection
                if ((awake_ev.events & EPOLLRDHUP) == EPOLLRDHUP) {
                    /*** user defined abnormal connection close handle ***/
                    if (err_handle) {
                        err_handle(awake_ev.data.fd,
                                   &cli_addr[awake_ev.data.fd],
                                   &cli_socklen[awake_ev.data.fd]);
                    }

                    // delete it from epoll list
                    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, awake_ev.data.fd,
                                  &awake_ev) == -1) {
                        // TODO: log
                        printf(
                            "[Warning] epoll delete abnormal closed connection "
                            "failed\n");
                        continue;
                    }
                    printf("[Info] close connection with fd %d\n",
                           awake_ev.data.fd);
                    close(events[i].data.fd);

                    continue;
                }

                // handle normal connection
                if ((awake_ev.events & EPOLLIN) == EPOLLIN) {
                    if (!in_handle) {
                        continue;
                    }

                    /*** user defined tcp stream handle ***/
                    serv_verb_t* verb = (serv_verb_t*)in_handle(
                        awake_ev.data.fd, &cli_addr[awake_ev.data.fd],
                        &cli_socklen[awake_ev.data.fd]);

                    if (verb == NULL) {
                        continue;
                    }

                    switch (*verb) {
                        // TODO: rebuild below codes. it duplicates.
                        case VERB_CLOSE_CONN:
                            if (epoll_ctl(epollfd, EPOLL_CTL_DEL,
                                          awake_ev.data.fd, &awake_ev) == -1) {
                                // TODO: log
                                printf(
                                    "[Warning] epoll delete abnormal closed "
                                    "connection "
                                    "failed\n");
                                continue;
                            }
                            printf("[Info] close connection with fd %d\n",
                                   awake_ev.data.fd);
                            close(events[i].data.fd);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    return NULL;
}