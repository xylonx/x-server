#include "resp.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 65535

// char resp_buf[65535] = {0};
char time_buf[30];

void msg_ok(int connfd, const char* header, const char* body) {
    char write_buf[WRITE_BUFFER_SIZE];
    char* cursor = write_buf;
    int write_byte, remain_byte = WRITE_BUFFER_SIZE;
    bzero(write_buf, WRITE_BUFFER_SIZE);

    write_byte = snprintf(cursor, remain_byte, "HTTP/1.1 200 OK\r\n", NULL);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = basic_resp_header(cursor, remain_byte);
    cursor += write_byte;
    remain_byte -= write_byte;

    if (header != NULL) {
        write_byte = snprintf(cursor, remain_byte, header, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    write_byte = snprintf(cursor, remain_byte, "Content-Length: %lu\r\n",
                          body ? strlen(body) : 0);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = snprintf(cursor, remain_byte, "\r\n", NULL);
    cursor += write_byte;
    remain_byte -= write_byte;

    // if buffer is full, send it first.
    if (remain_byte <= 0) {
        if (write(connfd, write_buf, WRITE_BUFFER_SIZE) == -1) {
            // TODO: log
            printf(
                "[Error] writing header to response file descriptor failed.\n");
            return;
        }
        cursor = write_buf;
        remain_byte = WRITE_BUFFER_SIZE;
        bzero(write_buf, WRITE_BUFFER_SIZE);
    }

    // wirte body
    // just for test
    if (body != NULL) {
        write_byte = snprintf(cursor, remain_byte, body, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    if (write(connfd, write_buf, WRITE_BUFFER_SIZE - remain_byte) == -1) {
        // TODO: log
        printf("[Error] sending response to client failed.\n");
        return;
    }

    return;

err:
    // TODO: log
    printf("[Error] write header into buf failed.\n");
    return;
}

void msg_not_allow(int connfd, const char* header, const char* body) {
    char write_buf[WRITE_BUFFER_SIZE];
    char* cursor = write_buf;
    int write_byte, remain_byte = WRITE_BUFFER_SIZE;
    bzero(write_buf, WRITE_BUFFER_SIZE);

    write_byte = snprintf(cursor, remain_byte,
                          "HTTP/1.1 405 Method Not Allowed\r\n", NULL);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = basic_resp_header(cursor, remain_byte);
    cursor += write_byte;
    remain_byte -= write_byte;

    if (header != NULL) {
        write_byte = snprintf(cursor, remain_byte, header, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    write_byte = snprintf(cursor, remain_byte, "Content-Length: %lu\r\n",
                          body ? strlen(body) : 0);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = snprintf(cursor, remain_byte, "\r\n", NULL);
    cursor += write_byte;
    remain_byte -= write_byte;

    // if buffer is full, send it first.
    if (remain_byte <= 0) {
        if (write(connfd, write_buf, WRITE_BUFFER_SIZE) == -1) {
            // TODO: log
            printf(
                "[Error] writing header to response file descriptor failed.\n");
            return;
        }
        cursor = write_buf;
        remain_byte = WRITE_BUFFER_SIZE;
        bzero(write_buf, WRITE_BUFFER_SIZE);
    }

    // wirte body
    if (body != NULL) {
        write_byte = snprintf(cursor, remain_byte, body, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    if (write(connfd, write_buf, WRITE_BUFFER_SIZE - remain_byte) == -1) {
        // TODO: log
        printf("[Error] sending response to client failed.\n");
        return;
    }

    return;

err:
    // TODO: log
    printf("[Error] write header into buf failed.\n");
    return;
}

void msg_not_found(int connfd, const char* header, const char* body) {
    char write_buf[WRITE_BUFFER_SIZE];
    char* cursor = write_buf;
    int write_byte, remain_byte = WRITE_BUFFER_SIZE;
    bzero(write_buf, WRITE_BUFFER_SIZE);

    write_byte =
        snprintf(cursor, remain_byte, "HTTP/1.1 404 Not Found\r\n", NULL);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = basic_resp_header(cursor, remain_byte);
    cursor += write_byte;
    remain_byte -= write_byte;

    if (header != NULL) {
        write_byte = snprintf(cursor, remain_byte, header, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    write_byte = snprintf(cursor, remain_byte, "Content-Length: %lu\r\n",
                          body ? strlen(body) : 0);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = snprintf(cursor, remain_byte, "\r\n", NULL);
    cursor += write_byte;
    remain_byte -= write_byte;

    // if buffer is full, send it first.
    if (remain_byte <= 0) {
        if (write(connfd, write_buf, WRITE_BUFFER_SIZE) == -1) {
            // TODO: log
            printf(
                "[Error] writing header to response file descriptor failed.\n");
            return;
        }
        cursor = write_buf;
        remain_byte = WRITE_BUFFER_SIZE;
        bzero(write_buf, WRITE_BUFFER_SIZE);
    }

    // wirte body
    remain_byte -= write_byte;
    if (body != NULL) {
        write_byte = snprintf(cursor, remain_byte, body, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    if (write(connfd, write_buf, WRITE_BUFFER_SIZE - remain_byte) == -1) {
        // TODO: log
        printf("[Error] sending response to client failed.\n");
        return;
    }

    return;

err:
    // TODO: log
    printf("[Error] write header into buf failed.\n");
    return;
}

void msg_forbidden(int connfd, const char* header, const char* body) {
    char write_buf[WRITE_BUFFER_SIZE];
    char* cursor = write_buf;
    int write_byte, remain_byte = WRITE_BUFFER_SIZE;
    bzero(write_buf, WRITE_BUFFER_SIZE);

    write_byte =
        snprintf(cursor, remain_byte, "HTTP/1.1 403 Forbidden\r\n", NULL);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = basic_resp_header(cursor, remain_byte);
    cursor += write_byte;
    remain_byte -= write_byte;

    if (header != NULL) {
        write_byte = snprintf(cursor, remain_byte, header, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    write_byte = snprintf(cursor, remain_byte, "Content-Length: %lu\r\n",
                          body ? strlen(body) : 0);
    if (write_byte < 0) {
        goto err;
    }
    cursor += write_byte;
    remain_byte -= write_byte;

    write_byte = snprintf(cursor, remain_byte, "\r\n", NULL);
    cursor += write_byte;
    remain_byte -= write_byte;

    // if buffer is full, send it first.
    if (remain_byte <= 0) {
        if (write(connfd, write_buf, WRITE_BUFFER_SIZE) == -1) {
            // TODO: log
            printf(
                "[Error] writing header to response file descriptor failed.\n");
            return;
        }
        cursor = write_buf;
        remain_byte = WRITE_BUFFER_SIZE;
        bzero(write_buf, WRITE_BUFFER_SIZE);
    }

    // wirte body
    if (body != NULL) {
        write_byte = snprintf(cursor, remain_byte, body, NULL);
        if (write_byte < 0) {
            goto err;
        }
        cursor += write_byte;
        remain_byte -= write_byte;
    }

    if (write(connfd, write_buf, WRITE_BUFFER_SIZE - remain_byte) == -1) {
        // TODO: log
        printf("[Error] sending response to client failed.\n");
        return;
    }

    return;

err:
    // TODO: log
    printf("[Error] write header into buf failed.\n");
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