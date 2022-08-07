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

#ifndef HTTP_COMMON_H
#define HTTP_COMMON_H

#include <string>

#include "except.h"
#include "spdlog/spdlog.h"

namespace xserver {

enum HTTPMethod { kGET = 0, kPOST, kPUT, kDELETE };

inline std::string ToString(HTTPMethod method) {
    switch (method) {
        case kGET: return "GET";
        case kPOST: return "POST";
        case kPUT: return "PUT";
        case kDELETE: return "DELETE";
    }
}

class RequestException : public XServerExcept {
public:
    explicit RequestException(HTTPMethod method, const std::string& uri, const std::string& remote_addr,
                              const std::string& hint) {
        message_ = fmt::format("{} {} remote: {} hint: {}", method, uri, remote_addr, hint);
    };
};

}  // namespace xserver

#endif /* HTTP_COMMON_H */