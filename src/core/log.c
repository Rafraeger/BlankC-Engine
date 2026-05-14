#include "core/log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

static FILE* s_log_file = NULL;
static LogLevel s_min_level = LOG_LEVEL_INFO;

static const char* level_str(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO ";
        case LOG_LEVEL_WARN:  return "WARN ";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        default:              return "?????";
    }
}

void log_init(const char* log_file) {
    if (log_file) {
        s_log_file = fopen(log_file, "w");
    }
    log_info("Logger initialized");
}

static void log_write(LogLevel level, const char* fmt, va_list args) {
    if (level < s_min_level) return;

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);

    char header[128];
    snprintf(header, sizeof(header), "[%s] [%s] ", time_buf, level_str(level));

    fprintf(stdout, "%s", header);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (s_log_file) {
        va_list args_copy;
        va_copy(args_copy, args);
        fprintf(s_log_file, "%s", header);
        vfprintf(s_log_file, fmt, args_copy);
        fprintf(s_log_file, "\n");
        fflush(s_log_file);
        va_end(args_copy);
    }
}

void log_debug(const char* fmt, ...) { va_list args; va_start(args, fmt); log_write(LOG_LEVEL_DEBUG, fmt, args); va_end(args); }
void log_info(const char* fmt, ...)  { va_list args; va_start(args, fmt); log_write(LOG_LEVEL_INFO, fmt, args); va_end(args); }
void log_warn(const char* fmt, ...)   { va_list args; va_start(args, fmt); log_write(LOG_LEVEL_WARN, fmt, args); va_end(args); }
void log_error(const char* fmt, ...)  { va_list args; va_start(args, fmt); log_write(LOG_LEVEL_ERROR, fmt, args); va_end(args); }
void log_fatal(const char* fmt, ...)  { va_list args; va_start(args, fmt); log_write(LOG_LEVEL_FATAL, fmt, args); va_end(args); }

void log_shutdown(void) {
    if (s_log_file) {
        fclose(s_log_file);
        s_log_file = NULL;
    }
}
