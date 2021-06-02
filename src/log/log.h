// TODO: use union log
#ifndef X_SERVER_LOG_H
#define X_SERVER_LOG_H
#include <stdarg.h>

void log_println(const char* format, ...);

void log_fatal(const char* format, ...);

#endif