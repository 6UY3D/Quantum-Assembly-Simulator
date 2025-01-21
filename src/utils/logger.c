#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

static LogLevel current_level = LOG_LEVEL_INFO;

static const char* level_to_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        default:              return "UNKNOWN";
    }
}

void set_log_level(LogLevel level) {
    current_level = level;
}

void log_message(LogLevel level, const char* format, ...) {
    if (level < current_level) {
        return; // Discard messages below the current log level
    }

    // Optional: include a timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);

    // Print log level and timestamp
    fprintf(stderr, "[%s] [%s] ", time_buf, level_to_string(level));

    // Print user message
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    // End with newline if not already
    size_t len = strlen(format);
    if (len == 0 || format[len - 1] != '\n') {
        fprintf(stderr, "\n");
    }
}
