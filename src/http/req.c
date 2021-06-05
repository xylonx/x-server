#include "req.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "conn.h"
#include "resp.h"

regex_t method_reg, conn_reg, header_reg;
const char* method_reg_pattern = "^(\\w+)\\s+(.+)\\s+HTTP.+\r$";
const size_t method_reg_nmatch = 3;
const char* conn_reg_pattern = "^Connection: (.+)\r$";
const size_t conn_reg_nmatch = 2;
const char* header_reg_pattern = "^.+:\\s+.+\r$";
const size_t header_reg_nmatch = 1;

const serv_verb_t close_verb = VERB_CLOSE_CONN;

// return the first line offset
// if don't contains a new line, return -1
// the line in buf is [buf, buf+offset)
int get_line(const char* buf, unsigned long buffer_size);
int start_at(const char* str, const char* pattern);
void init_regex();

void* get_hadle(int connfd);
void* post_handle(int connfd);
void* delete_handle(int connfd);
void file_error_handle(int connfd, int err);

void init_router_handle() { init_regex(); }

void* http_req_handle(int connfd, struct sockaddr* cli_addr,
                      socklen_t* cli_socklen) {
    char buffer[READ_BUFFER_SIZE], uri_buffer[URI_BUFFER_SIZE],
        body_buffer[BODY_BUFFER_SIZE];
    char *cursor = buffer,
         *uri_sp = uri_buffer + strlen(conf->server.root_path);
    int read_byte, line_offset, first_line = 1;
    bzero(buffer, READ_BUFFER_SIZE);
    bzero(uri_buffer, URI_BUFFER_SIZE);
    bzero(body_buffer, BODY_BUFFER_SIZE);

    // set URI prefix
    strcpy(uri_buffer, conf->server.root_path);
    if (*(uri_sp - 1) == '/') {
        uri_sp--;
    }

    http_method_t method;
    http_conn_status_t status = CONN_CLOSE;
    int is_req_broken = 0;

    // read bytes from connfd
    while ((read_byte = read(connfd, buffer, READ_BUFFER_SIZE)) != -1) {
        cursor = buffer;

        // get body position
        char* body_position = strstr(buffer, "\r\n\r\n") + 4;

        // handle http header message one line per time
        while (cursor < body_position &&
               (line_offset = get_line(cursor, strlen(cursor))) > 0) {
            // handle http state. include method and uri
            if (first_line) {
                first_line = !first_line;
                regmatch_t pmatch[method_reg_nmatch];
                if (regexec(&method_reg, cursor, method_reg_nmatch, pmatch, 0) <
                    0) {
                    // request is broken
                    is_req_broken = 1;
                    break;
                }

                // get method
                if (start_at(cursor + pmatch[1].rm_so, "GET")) {
                    method = METHOD_GET;
                } else if (start_at(cursor + pmatch[1].rm_so, "POST")) {
                    method = METHOD_POST;
                } else if (start_at(cursor + pmatch[1].rm_so, "DELETE")) {
                    method = METHOD_DELETE;
                } else {
                    // unsupported method
                    method = NOT_ALLOW_METHOD;
                }

                // get uri
                strncpy(uri_sp, cursor + pmatch[2].rm_so,
                        pmatch[2].rm_eo - pmatch[2].rm_so);
                uri_sp += pmatch[2].rm_eo - pmatch[2].rm_so;
                if (*(uri_sp - 1) == '/') {
                    // set default file end
                    strncpy(uri_sp, conf->server.default_index,
                            strlen(conf->server.default_index));
                }
            }
            // handle header
            else {
                regmatch_t header_match[1];
                if (cursor < body_position - 4 &&
                    regexec(&header_reg, cursor, 1, header_match, 0) < 0) {
                    is_req_broken = 1;
                    break;
                }
                // handle Connection header
                if (start_at(cursor, "Connection:")) {
                    regmatch_t pmatch[2];
                    if (regexec(&conn_reg, cursor, 2, pmatch, 0) < 0) {
                        // TODO:log
                        printf(
                            "[Warning] get connection status from http header "
                            "failed\n");
                        continue;
                    }

                    // FIXME:
                    if (!strncmp(cursor + pmatch[1].rm_so, "keep-alive", 10)) {
                        status = CONN_KA;
                    }
                }
            }

            cursor += line_offset;
        }

        if (is_req_broken) {
            msg_bad_request(connfd, NULL, NULL);
            return (void*)&close_verb;
        }

        /*** return message ***/
        void* rtn = status == CONN_KA ? NULL : (void*)&close_verb;

        FILE* file;
        // handle requst
        switch (method) {
            case NOT_ALLOW_METHOD:
                msg_not_allow(connfd, NULL, NULL);
                return rtn;
            case METHOD_GET:
                file = fopen(uri_buffer, "r");
                // File does not exist or does not contain read permission.
                if (file == NULL) {
                    int err = errno;
                    file_error_handle(connfd, err);
                    return rtn;
                }

                // FIXME: can't solve big file.
                fread(body_buffer, sizeof(char), BODY_BUFFER_SIZE, file);
                fclose(file);
                msg_ok(connfd, NULL, body_buffer);
                break;

            case METHOD_POST:
                file = fopen(uri_buffer, "w");
                if (file == NULL) {
                    int err = errno;
                    file_error_handle(connfd, err);
                    return rtn;
                }

                fwrite(body_position, sizeof(char), strlen(body_position),
                       file);
                fclose(file);
                msg_ok(connfd, NULL, NULL);
                break;

            case METHOD_DELETE:
                if (remove(uri_buffer) < 0) {
                    // TODO: log
                    printf("[Info] Delete file %s error.", uri_buffer);
                    int err = errno;
                    file_error_handle(connfd, err);
                    return rtn;
                }
                msg_ok(connfd, NULL, NULL);
                break;
        }
    }

    return NULL;
}

void file_error_handle(int connfd, int err) {
    switch (err) {
        case ENOENT:
            msg_not_found(connfd, NULL, NULL);
            return;

        // FIXME: for big file. set http header
        // Transfer-Encoding: chunked and omit Conntent-Length
        case EPERM:
            msg_forbidden(connfd, NULL, NULL);
            return;

        default:
            msg_internal_server_error(connfd, NULL, NULL);
    }
}

void init_regex() {
    regcomp(&method_reg, method_reg_pattern, REG_EXTENDED | REG_NEWLINE);
    regcomp(&conn_reg, conn_reg_pattern, REG_EXTENDED | REG_NEWLINE);
    regcomp(&header_reg, header_reg_pattern, REG_EXTENDED | REG_NEWLINE);
}

int get_line(const char* buf, unsigned long buffer_size) {
    char* cursor = (char*)buf;
    while (buf &&
           ((unsigned long)(cursor - buf) < buffer_size * sizeof(char)) &&
           *(cursor++) != '\n') {
    }

    if (cursor == NULL) {
        return -1;
    } else {
        return (size_t)(cursor - buf) / sizeof(char);
    }
}

int start_at(const char* str, const char* pattern) {
    return !strncmp(str, pattern, strlen(pattern));
}
