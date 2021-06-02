#include "resp.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 65535

// char resp_buf[65535] = {0};
char time_buf[30];

void msg_ok(int connfd, char* buffer, int buffer_size, const char* header,
            const char* body) {
    char* buf = buffer;
    int read_byte, remain_byte = buffer_size;

    /* below are http resp header */
    // write http resp status into buffer
    read_byte = snprintf(buf, remain_byte, "HTTP/1.1 200 OK\r\n", NULL);
    if (read_byte < 0) {
        // TODO: log
        printf("[Error] write header to response.\r\n");
        return;
    }
    buf += read_byte;
    remain_byte -= read_byte;

    // write basic header into buffer
    read_byte = basic_resp_header(buf, remain_byte);
    buf += read_byte;
    remain_byte -= read_byte;

    // writer user-define header into buffer
    if (header != NULL) {
        read_byte = snprintf(buffer, remain_byte, header, NULL);
        if (read_byte < 0) {
            printf("[Error] write header to response.\r\n");
            return;
        }
        buf += read_byte;
        remain_byte -= read_byte;
    }

    // write Content-Length into buffer
    read_byte = snprintf(buf, remain_byte, "Content-Length: %lu",
                         body ? strlen(body) : 0);
    if (read_byte < 0) {
        // TODO: log
        printf("[Error] write header to response.\r\n");
        return;
    }
    buf += read_byte;
    remain_byte -= read_byte;

    /* below are http resp body */

#ifndef DEBUG_NO_SOCKET
    write(connfd, buffer, buffer_size);
#endif

    printf("[Info] send http response:\n%s", buffer);
    return;
}

int basic_resp_header(char* buf, int buffer_size) {
    char* cursor = buf;
    int read_byte, total_bytes = 0;
    read_byte = snprintf(cursor, buffer_size - total_bytes, "Date: %s\r\n",
                         generate_time_string());

    if (read_byte < 0) {
        printf("[Error] write header to response.\r\n");
        return total_bytes;
    }
    cursor += read_byte;
    total_bytes += read_byte;

    read_byte = snprintf(cursor, buffer_size - total_bytes, "Sever: %s\r\n",
                         SERVER_VERSION);
    if (read_byte < 0) {
        // TODO: log
        printf("[Error] write header to response.\r\n");
        return total_bytes;
    }
    cursor += read_byte;
    total_bytes += read_byte;

    return total_bytes;
}

// just time info, without /r/n
char* generate_time_string() {
    time_t now;
    time(&now);
    struct tm* time_info = gmtime(&now);
    strftime(time_buf, sizeof(time_buf), DATE_FORMATTER, time_info);
    return time_buf;
}