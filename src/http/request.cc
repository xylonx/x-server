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

#include "http/common.h"
#include "http/handler/delete.h"
#include "http/handler/get.h"
#include "http/handler/post.h"
#include "http/handler/put.h"

namespace xserver {

void Request::ParseRequestLine() {}

void Request::ParseHeader() {}

void Request::InitHandler() {
    switch (method_) {
        case kGET: handler_ = new GETHandler(url_); return;
        case kPOST: handler_ = new POSTHandler(url_); return;
        case kPUT: handler_ = new PUTHandler(url_); return;
        case kDELETE: handler_ = new DELETEHandler(url_); return;
    }
};

}  // namespace xserver