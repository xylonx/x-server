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

#include "http/conn.h"

#include <unistd.h>

#include <cstring>

#include "spdlog/spdlog.h"

namespace xserver {

Conn::Conn(int fd, int read_buf_size, int write_buf_size)
    : fd_(fd), read_buffer_size_(read_buf_size), write_buffer_size_(write_buf_size), read_offset_(0), write_offset_(0) {
    read_buffer_  = new char[read_buf_size];
    read_start_   = read_buffer_;
    write_buffer_ = new char[write_buf_size];
    write_start_  = write_buffer_;
};

Conn::~Conn() {
    delete read_buffer_;
    delete write_buffer_;
};

const char *Conn::Read(int *len) {
    if (read_start_ + read_offset_ == read_buffer_ + read_buffer_size_) {
        read_start_ = read_buffer_;
    } else {
        read_start_ += read_offset_;
    }
    read_offset_ = 0;

    int read_bytes = read(fd_, read_start_, read_buffer_size_ - (read_start_ - read_buffer_));
    if (len != nullptr) {
        *len = read_bytes;
    }
    return read_start_;
};

const char *Conn::ReadLine(int *len) {
    // TODO(xylonx): to be impl
    return nullptr;
};

int Conn::Write(const char *data, int len) {
    // TODO(xylonx): to be impl
    return -1;
};

int Conn::Flush() {
    int written = write(fd_, write_start_, write_offset_);
    if (written < 0) {
        spdlog::error("write to fd {} from {} offset {} failed", fd_, fmt::ptr(write_start_), write_offset_);
        return written;
    }
    write_start_  = write_buffer_;
    write_offset_ = 0;
    return written;
};

void Conn::Close() const {
    if (close(fd_) < 0) {
        spdlog::error("close connection {} failed: {}", fd_, std::strerror(errno));
    }
};

}  // namespace xserver