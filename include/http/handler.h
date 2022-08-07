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

#ifndef HANDLER_H
#define HANDLER_H

#include "http/conn.h"

namespace xserver {

class Handler {
public:
    Handler(Conn* conn, const char* url) : conn_(conn), url_(url) {}

    virtual ~Handler() = default;

    virtual void Handle() = 0;

    virtual void Response() = 0;

protected:
    Conn* conn_;
    const char* url_;

private:
    /**
     * @brief Parse tons of headers, especially for header `Connection`, which whether or not keeping the tcp connection
     * depends on
     *
     */
    void ParseHeader();
};

}  // namespace xserver

#endif /* HANDLER_H */