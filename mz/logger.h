#ifndef __mz_logger_h__
#define __mz_logger_h__

#define LOGLEVEL_ERROR 1
#define LOGLEVEL_WARN 2
#define LOGLEVEL_INFO 3

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define CLEAN_ERRNO() (errno == 0 ? "None" : strerror(errno))
#define ERROR(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, CLEAN_ERRNO(), ##__VA_ARGS__)

#ifndef nodebug
#define DEBUG(M, ...) fprintf(stderr, "[DEBUG] %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(M, ...)
#endif

#ifndef loglevel
#define loglevel LOGLEVEL_INFO
#endif

#if loglevel >= LOGLEVEL_WARN
#define WARN(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, CLEAN_ERRNO(), ##__VA_ARGS__)
#else
#define WARN(M, ...)
#endif

#if loglevel == LOGLEVEL_INFO
#define INFO(M, ...) fprintf(stderr, "[INFO] (%s:%d:) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define INFO(M, ...)
#endif

#endif
