/*
 * This file is part of the xylonx distribution (https://github.com/xylonx).
 * Copyright (c) 2022 xylonx.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/epoll.h>
#include <sys/socket.h>

#include <cstring>

#include "const.h"
#include "except.h"
#include "globals.h"
#include "http/request.h"
#include "http/server.h"
#include "spdlog/spdlog.h"

namespace xserver {

HTTPServer::HTTPServer() {
    cli_addr_    = new struct sockaddr_in[kMAX_FD];
    cli_socklen_ = new socklen_t[kMAX_FD];
}

void HTTPServer::Serve(const char *addr, int port, const char *root, const char *index) {
    int listen_fd = ListenAndBind(addr, port);
    int ep        = epoll_create1(0);
    if (ep < 0) {
        throw XServerExcept(fmt::format("init epoll failed: {}", std::strerror(errno)));
    }

    // init first epoll event: TCP socket in
    struct epoll_event socket_in_event;
    socket_in_event.events  = EPOLLIN;
    socket_in_event.data.fd = listen_fd;

    if (epoll_ctl(ep, EPOLL_CTL_ADD, listen_fd, &socket_in_event) < 0) {
        throw XServerExcept(fmt::format("monitor listen fd failed: {}", std::strerror(errno)));
    }

    spdlog::info("xserver start listen at {}:{}", addr, port);

    struct epoll_event events[multiplex_max_events];
    while (true) {
        int ready_num = epoll_wait(ep, events, multiplex_max_events, multiplex_wait_nsec * 10e6);
        if (ready_num < 0) {
            spdlog::warn("epoll get ready fd failed: ready fd: {} hint:{}", ready_num, std::strerror(errno));
            continue;
        }
        if (ready_num == 0) {
            continue;
        }

        for (int i = 0; i < ready_num; ++i) {
            int event_fd = events[i].data.fd;
            if (event_fd == listen_fd) {
                int connfd = accept4(listen_fd, reinterpret_cast<struct sockaddr *>(cli_addr_ + event_fd),
                                     cli_socklen_ + event_fd, SOCK_NONBLOCK);
                if (connfd < 0) {
                    spdlog::error("accept connection failed: {}", std::strerror(errno));
                    continue;
                }

                struct epoll_event conn_evt;
                conn_evt.events  = EPOLLIN | EPOLLRDHUP | EPOLLET;
                conn_evt.data.fd = connfd;
                if (epoll_ctl(ep, EPOLL_CTL_ADD, connfd, &conn_evt) < 0) {
                    spdlog::error("epoll mintor connfd {} failed: {}", connfd, std::strerror(errno));
                    continue;
                }

                spdlog::info("epoll monitor new connfd: {}", connfd);
                requests_[connfd] = new Request(connfd, kMAX_REQUEST_LINE_LEN, kMAX_HEADER_LEN, read_buffer_size,
                                                write_buffer_size, root, index);

                continue;
            }

            // handle tcp connection in
            auto flags  = events[i].events;
            auto connfd = event_fd;
            try {
                if (flags & EPOLLRDHUP) {  // NOLINT:readability-implicit-bool-conversion
                    HandleConnClientClose(connfd);
                } else if (flags & EPOLLIN) {  // NOLINT:readability-implicit-bool-conversion
                    HandleConnIn(connfd);
                } else {
                    spdlog::warn("connfd {} unkown event flag: {}", connfd, flags);
                }
            } catch (const XServerExcept &e) {
                spdlog::error("handler failed. connection {} will be close: {}", connfd, e.what());
                delete requests_[connfd];
            }
        }
    }
}

}  // namespace xserver