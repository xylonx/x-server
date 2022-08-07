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

#include <sys/errno.h>
#include <sys/event.h>

#include <cerrno>
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

// The API kqueue() performs like epoll_ctl + epoll_wait in Linux.
// Arguments changelist and nchanges are used to modify monitored file descriptor just like epoll_ctl does in Linux
// Arguments eventlist, nevents and timeout are used to return ready file descriptor just like epoll_wait does in Linux
void HTTPServer::Serve(const char *addr, int port, const char *root, const char *index) {
    int listen_fd = ListenAndBind(addr, port);
    int kq_fd     = kqueue();
    if (kq_fd < 0) {
        throw XServerExcept(fmt::format("init kqueue failed: {}", std::strerror(errno)));
    }

    // init kevent wait duration
    struct timespec duration {
        0, multiplex_wait_nsec,
    };

    // listen socket with EVFILT_READ, which means listening connection establish event
    struct kevent listen_enable_event;
    EV_SET(&listen_enable_event, listen_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    if (kevent(kq_fd, &listen_enable_event, 1, nullptr, 0, nullptr) < 0) {
        throw XServerExcept(fmt::format("monitor listen fd failed: {}", std::strerror(errno)));
    }

    spdlog::info("xserver start listen at {}:{}", addr, port);

    struct kevent events[multiplex_max_events];
    while (true) {
        int ready_num = kevent(kq_fd, nullptr, 0, events, multiplex_max_events, &duration);  // 添加
        if (ready_num < 0) {
            spdlog::warn("kevent get read fd failed: {} {}", ready_num, std::strerror(errno));
            continue;
        }
        if (ready_num == 0) {
            continue;
        }

        // handle ready fd
        for (int i = 0; i < ready_num; ++i) {
            int event_fd = events[i].ident;
            // means new connection in
            if (event_fd == listen_fd) {
                int connfd = accept(listen_fd, reinterpret_cast<struct sockaddr *>(cli_addr_ + event_fd),
                                    cli_socklen_ + event_fd);
                if (connfd < 0) {
                    spdlog::error("accept connection failed: {}", std::strerror(errno));
                    continue;
                }
                struct kevent e;
                EV_SET(&e, connfd, EVFILT_READ | EV_EOF, EV_ADD | EV_ENABLE, 0, 0, nullptr);
                if (kevent(kq_fd, &e, 1, nullptr, 0, nullptr) < 0) {
                    spdlog::error("kevent monitor connfd {} failed: ", connfd, std::strerror(errno));
                    continue;
                }

                spdlog::info("kevent monitor new connfd {}", connfd);
                requests_[connfd] = new Request(connfd, kMAX_REQUEST_LINE_LEN, kMAX_HEADER_LEN, read_buffer_size,
                                                write_buffer_size, root, index);

                continue;
            }

            // handle connection fd read ready or write ready event
            auto flags  = events[i].flags;
            auto connfd = event_fd;
            try {
                if (flags & EV_EOF) {  // NOLINT:readability-implicit-bool-conversion
                    HandleConnClientClose(connfd);
                }
                // FIXME(xylonx): incorrect event flag for monitoring fd close event
                else if (flags & EVFILT_READ) {  // NOLINT:readability-implicit-bool-conversion
                    HandleConnIn(connfd);
                } else {
                    spdlog::warn("connfd {} unkown event flag: {}", connfd, flags);
                }
            } catch (const XServerExcept &e) {
                spdlog::error("handler failed. connection {} will be closed. {}", connfd, e.what());
                // When close the connfd, kqueue will remove the fd from its listening list. Therefore, there is no
                // need to remove it from kqueue manually
                delete requests_[connfd];
            }
        }
    }
}

}  // namespace xserver