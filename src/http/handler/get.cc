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

#include "http/handler/get.h"

#include <unistd.h>

#include <cstring>
#include <string>

#include "except.h"
#include "spdlog/spdlog.h"

namespace xserver {

GETHandler::~GETHandler() = default;

void GETHandler::Handle() {
    // TODO(xylonx): below codes are just used for read POC. It should be change to read impl
    int read_bytes = 0;
    const char* x  = conn_->Read(&read_bytes);
    if (read_bytes < 0) {
        throw XServerExcept(fmt::format("read from conn failed: {}", std::strerror(errno)));
    }

    spdlog::info("read from conn: {}", x);
}

void GETHandler::Response() {}

}  // namespace xserver