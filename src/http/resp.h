#ifndef X_SERVER_RESP_H
#define X_SERVER_RESP_H

// #define DEBUG_NO_SOCKET

#define WRITE_BUFFER_SIZE 65536

#define DATE_FORMATTER "%a, %d %b %Y %H:%M:%S GMT"
#define SERVER_VERSION "X-SERVER/0.0.1(Alpha) (Linux)"

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_BAD_REQUEST 400
#define HTTP_STATUS_FORBIDDEN 403
#define HTTP_STATUS_NOT_FOUND 404
#define HTTP_STATUS_METHOD_NOT_ALLOWED 405
#define HTTP_STATUS_INTERNAL_SERVER_ERROR 500

int basic_resp_header(char* buf, int buffer_size);

void resp_msg(int connfd, int status, const char* header, const char* body);
void msg_ok(int connfd, const char* header, const char* body);
void msg_bad_request(int connfd, const char* header, const char* body);
void msg_forbidden(int connfd, const char* header, const char* body);
void msg_not_found(int connfd, const char* header, const char* body);
void msg_not_allow(int connfd, const char* header, const char* body);
void msg_internal_server_error(int connfd, const char* header,
                               const char* body);

char* generate_time_string();

#endif