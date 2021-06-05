#ifndef X_SERVER_CONF_H
#define X_SERVER_CONF_H

// to control debug mode
// #define _DEBUG_MODE
// #define _DEBUG_CONFIG_PATH "~/repo/x-server/conf.json"

// config struct
struct _conf_server_t;
typedef struct _conf_server_t conf_server_t;

struct _conf_t;
typedef struct _conf_t* conf_t;

// global conf variables
extern conf_t conf;

// load config from config file whose
// path is read from env variable `env_file_path`
void init_conf(conf_t* conf);

struct _conf_server_t {
    char bind_addr[16];
    int bind_port;
    char root_path[256];
};

struct _conf_t {
    conf_server_t server;
};

#endif