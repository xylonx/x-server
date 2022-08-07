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

#ifndef HTTP_HANDLER_PUT
#define HTTP_HANDLER_PUT

#include "http/common.h"
#include "http/handler.h"

namespace xserver {

class PUTHandler : public Handler {
public:
    PUTHandler(Conn* conn, const char* url) : Handler(conn, url) {}

    ~PUTHandler() override;

    void Handle() override;

    void Response() override;
};

}  // namespace xserver

#endif /* HTTP_HANDLER_PUT */