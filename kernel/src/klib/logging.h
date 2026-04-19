#ifndef KLIB_LOGGING_H
#define KLIB_LOGGING_H

// Wrappers around `log()`
#define debug(...)         log(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define info(...)          log(LOG_LEVEL_INFO, __VA_ARGS__)
#define warn(...)          log(LOG_LEVEL_WARNING, __VA_ARGS__)
#define error(...)         log(LOG_LEVEL_ERROR, __VA_ARGS__)
#define fatal(...)         log(LOG_LEVEL_FATAL, __VA_ARGS__)

// Different levels of log messages
// * LOG_LEVEL_DEBUG    : information for the developer
// * LOG_LEVEL_INFO     : information or note for everyone
// * LOG_LEVEL_WARNING  : somehting bad happened but it is not a big issue
// * LOG_LEVEL_ERROR    : something bad happened, the task should stop
// * LOG_LEVEL_FATAL    : something very very bad happened
enum log_level {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL
};

void log(enum log_level level, const char *fmt, ...);

#endif /* KLIB_LOGGING_H */