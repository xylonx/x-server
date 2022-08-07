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

#include "const.h"
#include "except.h"
#include "globals.h"
#include "http/request.h"
#include "http/server.h"
#include "spdlog/spdlog.h"

namespace xserver {

HTTPServer::HTTPServer() {
    cli_addr_    = new (struct sockaddr_in);
    cli_socklen_ = new socklen_t;
}

// TODO(xylonx): to be impl
void HTTPServer::Serve(const char *addr, int port, const char *root, const char *index) {}

}  // namespace xserver