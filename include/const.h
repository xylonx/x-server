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

#ifndef CONST_H
#define CONST_H

namespace xserver {

const int kMAX_FD = 12;

const int kMAX_URL_LEN = 256;
// max RequestLine: `DELETE ${url} HTTP/1.1`
const int kMAX_REQUEST_LINE_LEN = kMAX_URL_LEN + 6 + 8 + 2;
const int kMAX_HEADER_LEN       = 1024;

}  // namespace xserver

#endif /* end CONST_H */