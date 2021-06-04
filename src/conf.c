#include "conf.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "cJSON.h"

void init_conf(conf_t* conf) {
    if (conf != NULL && *conf != NULL) {
        free(*conf);
    }

    conf_t conf_addr = (conf_t)malloc(sizeof(struct _conf_t));
    *conf = conf_addr;
    if (conf_addr == NULL) {
        // TODO: log
        printf("[Error] Cannot allocate\n");
        exit(EXIT_FAILURE);
    }
    bzero(conf_addr, sizeof(struct _conf_t));

// get env_file_path
#ifdef _DEBUG_CONFIG_PATH
    const char* env_file_path = _DEBUG_CONFIG_PATH;
#else
    const char* env_file_path = getenv("env_file_path");
#endif

    if (env_file_path == NULL) {
        // TODO: use uninon log function
        printf("load environment variable `env_file_path` failed\n");
        exit(EXIT_FAILURE);
    }

    // set conf value
    FILE* fp = fopen(env_file_path, "r");
    if (fp == NULL) {
        // TODO: use uninon log function
        printf("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char* buffer = NULL;
    size_t len;
    ssize_t bytes_read = getdelim(&buffer, &len, '\0', fp);
    if (bytes_read < 0) {
        // TODO: use uninon log function
        printf("Error while reading the file.\n");
        exit(EXIT_FAILURE);
    }

    // FIXME: just for test
    // printf("file content: %s\n", buffer);

    // parse json by cJSON package
    cJSON* root = cJSON_Parse(buffer);
    cJSON* server = cJSON_GetObjectItem(root, "server");
    cJSON* item;
    item = cJSON_GetObjectItem(server, "bind_addr");
    memcpy(conf_addr->server.bind_addr, item->valuestring,
           strlen(item->valuestring));
    item = cJSON_GetObjectItem(server, "bind_port");
    conf_addr->server.bind_port = item->valueint;
    item = cJSON_GetObjectItem(server, "root_path");
    memcpy(conf_addr->server.root_path, item->valuestring,
           strlen(item->valuestring));
}