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

#include "http/server.h"

#include <arpa/inet.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>

#include "except.h"

namespace xserver {

void HTTPServer::Serve(const char *addr, int port, const char *root, const char *index) {}

int HTTPServer::ListenAndBind(const char *addr, int port) {
    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_fd < 0) {
        throw XServerExcept(std::string("create socket failed ") + std::strerror(errno));
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    // TODO(xylonx): throw?
    inet_pton(AF_INET, addr, &sockaddr.sin_addr);
    sockaddr.sin_port = htons(port);

    if (bind(listen_fd, reinterpret_cast<struct sockaddr *>(&sockaddr), sizeof(sockaddr)) < 0) {
        throw XServerExcept(std::string("bind socket failed ") + std::strerror(errno));
    }

    // TODO(xylonx): control listen buffer size dynamically
    if (listen(listen_fd, 5) < 0) {
        throw XServerExcept(std::string("listen socket failed ") + std::strerror(errno));
    }

    return listen_fd;
};

}  // namespace xserver