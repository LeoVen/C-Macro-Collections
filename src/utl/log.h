/**
 * utl/log.h
 *
 * Creation Date: 21/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Simple logging macros
 * You can toggle colors defining the macro CMC_LOG_COLOR but be sure that
 * your terminal supports ANSI color escape codes.
 *
 * You can customize the logging utility by accessing cmc_log_config and
 * directly modifying its values to better suit your needs.
 */

/**
 * What each error code means inside the DEV Collections
 *
 * TRACE - Trace is used to describe the inner workings of the function or an
 *         algorithm. It can be useful when trying to find for an error in the
 *         algorithm or corner cases.
 * DEBUG - Mainly used for debugging, tracking certain variables or anything
 *         useful to help visualizing what your program is doing.
 *  INFO - Info can be used as a heads up. It is also used to track the main
 *         collection's allocation and deallocation.
 *  WARN - Warnings are attributed to all cases where the function can recover
 *         from and they are usually treated by it. In most cases the user can
 *         treat these warnings himself.
 * ERROR - Something really bad happened. Your program will not crash yet but
 *         depending on what comes next, it will.
 * FATAL - This will probably be the last logging message before your program
 *         crashes or goes into madness. Fatal errors are commonly attributed
 *         to dereferencing NULL pointers.
 *
 * Log Level
 *
 * TRACE - 1
 * DEBUG - 2
 *  INFO - 3
 *  WARN - 4
 * ERROR - 5
 * FATAL - 6
 *
 * How the Log Level is calculated for tlevel and flevel
 *
 * - If the log level is 0 all logs are enabled.
 * - If the log level (X) is positive:
 *      - All levels less than X will be disabled
 *      - If X is greater than the level of FATAL, all logs are disabled
 * - If the log level (X) is negative:
 *      - All levels greater than X are enabled
 *      - If X is less than the level of FATAL, all logs are enabled
 */

#ifndef CMC_UTL_LOG_H
#define CMC_UTL_LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

static const char *cmc_log_names[] = { "TRACE", "DEBUG", "INFO",
                                       "WARN",  "ERROR", "FATAL" };

#ifdef CMC_LOG_COLOR
static const char *cmc_log_color[] = { "\x1b[94m", "\x1b[36m", "\x1b[32m",
                                       "\x1b[33m", "\x1b[31m", "\x1b[35m" };
#endif

enum cmc_log_type
{
    CMC_LOG_TRACE = 1,
    CMC_LOG_DEBUG = 2,
    CMC_LOG_INFO = 3,
    CMC_LOG_WARN = 4,
    CMC_LOG_ERROR = 5,
    CMC_LOG_FATAL = 6
};

static struct
{
    int tlevel;    /* Terminal Level */
    int flevel;    /* File Level */
    bool tenabled; /* Terminal Output Enabled */
    bool fenabled; /* File Output Enabled */
    bool enabled;  /* Logging enabled */
    FILE *file;    /* File for Output */

} cmc_log_config = { 0, 0, true, true, true, NULL };

#define cmc_log_trace(fmt, ...) \
    cmc_log(CMC_LOG_TRACE, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define cmc_log_debug(fmt, ...) \
    cmc_log(CMC_LOG_DEBUG, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define cmc_log_info(fmt, ...) \
    cmc_log(CMC_LOG_INFO, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define cmc_log_warn(fmt, ...) \
    cmc_log(CMC_LOG_WARN, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define cmc_log_error(fmt, ...) \
    cmc_log(CMC_LOG_ERROR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define cmc_log_fatal(fmt, ...) \
    cmc_log(CMC_LOG_FATAL, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

static void cmc_log(enum cmc_log_type log, const char *filename,
                    const char *funcname, unsigned line, const char *fmt, ...)
{
    if (!cmc_log_config.enabled)
        return;

    if (cmc_log_config.tenabled)
    {
        int tl = cmc_log_config.tlevel;

        if (tl > 0 && (int)log < tl)
            goto skip;
        if (tl < 0 && ((int)log * -1) < tl)
            goto skip;

        int i = (int)log - 1;

        time_t t = time(NULL);
        struct tm *lt = localtime(&t);

        char time[32];
        time[strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", lt)] = '\0';

#ifdef CMC_LOG_COLOR
        fprintf(stderr, "%s %s%5s\x1b[0m at\x1b[90m %s:%s:%u\x1b[0m | ", time,
                cmc_log_color[i], cmc_log_names[i], filename, funcname, line);
#else
        fprintf(stderr, "%s %5s at %s:%s:%u | ", time, cmc_log_names[i],
                filename, funcname, line);
#endif

        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        fprintf(stderr, "\n");
        fflush(stderr);
    }

skip:

    if (cmc_log_config.file && cmc_log_config.fenabled)
    {
        int fl = cmc_log_config.flevel;

        if (fl > 0 && (int)log < fl)
            return;
        if (fl < 0 && ((int)log * -1) < fl)
            return;

        int i = (int)log - 1;

        time_t t = time(NULL);
        struct tm *lt = localtime(&t);

        char time[32];
        time[strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", lt)] = '\0';

        fprintf(cmc_log_config.file, "%s %5s at %s:%s:%u | ", time,
                cmc_log_names[i], filename, funcname, line);

        va_list file_args;
        va_start(file_args, fmt);
        vfprintf(cmc_log_config.file, fmt, file_args);
        va_end(file_args);

        fprintf(cmc_log_config.file, "\n");
        fflush(cmc_log_config.file);
    }
}

#endif /* CMC_UTL_LOG_H */
