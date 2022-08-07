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

#ifndef HTTP_CONN_H
#define HTTP_CONN_H

namespace xserver {

class Conn {
public:
    Conn(int fd, int read_buf_size, int write_buf_size);
    ~Conn();

    /**
     * @brief  read data from `fd_`. return the read data and length
     *
     * @param len return as read length
     * @return const char* read data
     */
    const char* Read(int* len);

    /**
     * @brief read a line from data
     *
     * @param len return as read length
     * @return const char* read data
     */
    const char* ReadLine(int* len);

    /**
     * @brief write data to file descriptor. To gain maximal efficiency, it will write to write_buffer_ first. When
     * write_buffer_ is full, it will call Flush to flush the whole write_buffer_ to file descriptor
     *
     * @param data bytes to be written
     * @param len max len that the data should be written
     * @return written data length
     */
    int Write(const char* data, int len);

    /**
     * @brief Flush the bytes in write buffer to file descriptor
     *
     * @return written data length
     */
    int Flush();

    /**
     * @brief close the file descriptor
     *
     */
    void Close() const;

    // TODO(xylonx): add method to support zero-copy read & write

private:
    const int fd_;
    int read_buffer_size_;
    int write_buffer_size_;

    char* read_buffer_;  // read ring buffer
    char* read_start_;   // read ring buffer start pointer
    int read_offset_;    // read ring buffer read offset

    char* write_buffer_;  // write ring buffer
    char* write_start_;   // write ring buffer start pointer
    int write_offset_;    // write ring buffer written offset
};

}  // namespace xserver

#endif /* HTTP_CONN_H */