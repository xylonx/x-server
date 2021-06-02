#include <stdio.h>

#include "conf.h"
#include "conn.h"
#include "resp.h"

conf_t conf;

int main(int argc, char const* argv[]) {
    init_conf(&conf);

    int listenfd = start_listen();

    char buffer[65535] = {0};
    // for test
    msg_ok(12, buffer, 65535, NULL, NULL);

    return 0;
}
