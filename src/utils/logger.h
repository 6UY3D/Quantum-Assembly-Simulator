#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Logging levels enumeration
 */
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

/**
 * \brief Sets the global log level. Messages below this level will be discarded.
 * \param level The minimum level of messages to be logged
 */
void set_log_level(LogLevel level);

/**
 * \brief Logs a message with the specified level and format string.
 * \param level The log level
 * \param format printf-style format string
 * \param ... Arguments for the format string
 */
void log_message(LogLevel level, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_H */
