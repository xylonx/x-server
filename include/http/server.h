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

#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#include "config.h"
#include "const.h"
#include "http/request.h"

namespace xserver {

class HTTPServer {
public:
    HTTPServer();

    ~HTTPServer() {
        delete cli_addr_;
        delete cli_socklen_;
        for (auto &request : requests_) {
            delete request;
        }
    };

    /**
     * @brief listen and serve http server at given addr:port, with root and index option
     *
     * @param addr bind addr
     * @param port bind port
     * @param root static file host
     * @param index index file suffix like in Nginx. For example, only when /path and /path.${index} does both not
     * exist, will the server return 404
     */
    void Serve(const char *addr, int port, const char *root, const char *index);

private:
    Request *requests_[kMAX_FD];
    struct sockaddr_in *cli_addr_;
    socklen_t *cli_socklen_;

    /**
     * @brief create a socket and listen
     *
     * @param addr bind addr
     * @param port bind port
     * @return int listen fd
     */
    int ListenAndBind(const char *addr, int port);

    void HandleConnIn(int conn);

    void HandleConnClientClose(int conn);
};

}  // namespace xserver

#endif /* SERVER_H */