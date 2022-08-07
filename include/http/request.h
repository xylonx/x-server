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

#ifndef REQUEST_H
#define REQUEST_H

#include <unistd.h>

#include "except.h"
#include "http/common.h"
#include "http/conn.h"
#include "http/handler.h"

namespace xserver {

class Request {
public:
    /**
     * @brief Construct a new Request object
     *
     * @param conn_fd connection file descriptor
     * @param max_rl max read requestLine len
     * @param max_header max header len
     * @param read_buf_size read buffer size
     * @param write_buf_size write buffer size
     */
    Request(int conn_fd, int max_rl, int max_header, int read_buf_size, int write_buf_size, const char* root,
            const char* index);

    ~Request();

    /**
     * @brief Receive request from connection. parse method, url, http version and init the `handler_`. Moreover, the
     * actual body processing delegates to `handler_.Handle()`. Therefore, Request can be seen as HandlerFactory
     *
     * @throw XServerException when occurring when reading request from conn_fd_
     */
    void Receive();

    /**
     * @brief Response http connection. It is a simple wrap of `handler_.Response()`
     *
     * @throw XServerException when occurring when writing response to conn_fd_
     */
    void Response();

private:
    Conn* conn_;

    const char* root_;
    const char* index_;

    // HTTP method presented in RequestLine
    HTTPMethod method_;
    // HTTP url presented in RequestLine
    char* url_;
    // HTTP version presented in RequestLine
    char* http_version_;

    Handler* handler_;

    void ParseRequestLine();
};

}  // namespace xserver

#endif /* REQUEST_H */