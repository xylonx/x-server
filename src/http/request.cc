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

#include "http/request.h"

#include <unistd.h>

#include <cstring>

#include "except.h"
#include "http/common.h"
#include "http/handler/delete.h"
#include "http/handler/get.h"
#include "http/handler/post.h"
#include "http/handler/put.h"
#include "spdlog/spdlog.h"

namespace xserver {

Request::Request(int conn_fd, int max_rl, int max_header, int read_buf_size, int write_buf_size, const char* root,
                 const char* index)
    : root_(root), index_(index) {
    conn_         = new Conn(conn_fd, read_buf_size, write_buf_size);
    url_          = new char[max_rl];
    http_version_ = nullptr;  // reuse url_
    handler_      = nullptr;
}

Request::~Request() {
    delete url_;
    delete handler_;
    conn_->Close();
    // delete conn_;
}

void Request::ParseRequestLine() {
    // TODO(xylonx): read first line of requests

    // TODO(xylonx): init methods_, url_, http_version

    // HACK(xylonx): simple patch now
    method_ = kGET;
}

// void Request::ParseHeader() {}

void Request::Receive() {
    ParseRequestLine();

    switch (method_) {
        case kGET: handler_ = new GETHandler(conn_, url_); break;
        case kPOST: handler_ = new POSTHandler(conn_, url_); break;
        case kPUT: handler_ = new PUTHandler(conn_, url_); break;
        case kDELETE: handler_ = new DELETEHandler(conn_, url_); break;
    }

    handler_->Handle();
};

void Request::Response() { handler_->Response(); }

}  // namespace xserver