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

#ifndef EXCEPT_H
#define EXCEPT_H

#include <exception>
#include <string>
#include <utility>

namespace xserver {

class XServerExcept : public std::exception {
public:
    XServerExcept() = default;
    explicit XServerExcept(std::string str) : message_(std::move(str)) {}

    ~XServerExcept() override = default;

    const char* what() const noexcept override { return message_.c_str(); };

protected:
    std::string message_;
};

}  // namespace xserver

#endif /* EXCEPT_H */