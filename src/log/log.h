// TODO: use union log
#ifndef HTTPSERVER_LOG_H
#define HTTPSERVER_LOG_H
#include <stdarg.h>

void log_println(const char* format, ...);

void log_fatal(const char* format, ...);

#endif