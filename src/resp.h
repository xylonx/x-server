#ifndef X_SERVER_RESP_H
#define X_SERVER_RESP_H

#define DEBUG_NO_SOCKET

#define DATE_FORMATTER "%a, %d %b %Y %H:%M:%S GMT"
#define SERVER_VERSION "X-SERVER/0.0.1(Alpha) (Linux)"

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_BAD_REQUEST 400
#define HTTP_STATUS_NOT_FOUND 404
#define HTTP_STATUS_INTERNAL_SERVER 500

int basic_resp_header(char* buf, int buffer_size);
void msg_ok(int connfd, char* buffer, int buffer_size, const char* header,
            const char* body);
void msg_not_found(int connfd);
void msg_forbidden(int connfd);
void msg_internal_server_error(int connfd);

char* generate_time_string();

#endif