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

#include "http/common.h"
#include "http/handler.h"

namespace xserver {

class Request {
public:
    void ParseRequestLine();

    void ParseHeader();

    void InitHandler();

private:
    HTTPMethod method_;
    char* url_;
    char* http_version_;

    Handler* handler_;
};

}  // namespace xserver

#endif /* REQUEST_H */