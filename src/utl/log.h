/**
 * log.h
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
 * your terminal supports ANSI color escape codes
 */

/**
 * What each error code means
 *
 * TRACE - Trace is used to describe the inner workings of the function or an
 *         algorithm. It can be useful when trying to find for an error in the
 *         algorithm or an untreated case.
 * DEBUG - Mainly used for debugging, tracking certain variables or anything
 *         useful to help visualizing what your program is doing.
 *  INFO - Info can be used as a heads up. It is also used to track the main
 *         collection's allocation and deallocation.
 *  WARN - Warnings are attributed to all cases where the function can recover
 *         from and they are usually treated by it. In most cases the user can
 *         treat these warnings himself.
 * ERROR - Something really bad happened. Your program will not crash yet but
 *         depending on what comes next, it might crash indeed.
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
 * Functions API
 *
 * void cmc_log_tlevel(int level)
 *      Set level for the Terminal Output to stderr.
 * void cmc_log_flevel(int level)
 *      Set level for the File Output to FILE pointer.
 * void cmc_log_tenable(bool enabled)
 *      Enables or disables Terminal Output to stderr.
 * void cmc_log_fenable(bool enabled)
 *      Enables or disables File Output.
 * void cmc_log_file(FILE *file)
 *      Set a file for writing the logging messages to it.
 * void cmc_log_set(bool enabled)
 *      Sets on or off all the logging messages.
 *
 * How the Log Level is calculated
 *
 * - If the log level is 0 all logs are enabled.
 * - If the log level (X) is positive:
 *      - All levels less than X will be disabled
 *      - If X is greater than the level of FATAL, all logs are disabled
 * - If the log level (X) is negative:
 *      - ALl levels greater than X are enabled
 *      - If X is less than the level of FATAL, all logs are enabled
 */

#ifndef CMC_LOG_H
#define CMC_LOG_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

static const char *names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
static const char *color[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"};

static struct
{
    int tlevel;    /* Terminal Level */
    int flevel;    /* File Level */
    bool tenabled; /* Terminal Output Enabled */
    bool fenabled; /* File Output Enabled */
    FILE *file;    /* File for Output */
    bool enabled;  /* Logging enabled */
} log_config = {0, 0, true, true, NULL, true};

typedef enum log_type_e
{
    LOG_TRACE = 1,
    LOG_DEBUG = 2,
    LOG_INFO = 3,
    LOG_WARN = 4,
    LOG_ERROR = 5,
    LOG_FATAL = 6
} log_type;

#define log_trace(fmt, ...) cmc_log(LOG_TRACE, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__)
#define log_debug(fmt, ...) cmc_log(LOG_DEBUG, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__)
#define log_info(fmt, ...) cmc_log(LOG_INFO, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__)
#define log_warn(fmt, ...) cmc_log(LOG_WARN, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__)
#define log_error(fmt, ...) cmc_log(LOG_ERROR, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__)
#define log_fatal(fmt, ...) cmc_log(LOG_FATAL, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__)

static void cmc_log_tlevel(int terminal_level)
{
    log_config.tlevel = terminal_level;
}

static void cmc_log_flevel(int file_level)
{
    log_config.flevel = file_level;
}

static void cmc_log_tenable(bool enabled)
{
    log_config.tenabled = enabled;
}

static void cmc_log_fenable(bool enabled)
{
    log_config.fenabled = enabled;
}

static void cmc_log_file(FILE *file)
{
    log_config.file = file;
}

static void cmc_log_set(bool enabled)
{
    log_config.enabled = enabled;
}

static void cmc_log(log_type log, const char *filename, const char *funcname, int line, const char *fmt, ...)
{
    if (!log_config.enabled)
        return;

    if (log_config.tenabled)
    {
        if (log_config.tlevel < 0 && ((int)log * -1) < log_config.tlevel)
            return;
        else if (log_config.tlevel > 0 && (int)log < log_config.tlevel)
            return;

        int i = (int)log - 1;

        time_t t = time(NULL);
        struct tm *lt = localtime(&t);

        char time[16];
        time[strftime(time, sizeof(time), "%H:%M:%S", lt)] = '\0';

#if defined(CMC_LOG_COLOR)
        fprintf(stderr,
                "%s %s%5s\x1b[0m \x1b[90m%s at %s:%d:\x1b[0m ",
                time, color[i], names[i], filename, funcname, line);
#else
        fprintf(stderr,
                "%s %5s %s at %s:%d: ",
                time, names[i], filename, funcname, line);
#endif

        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        fprintf(stderr, "\n");
        fflush(stderr);
    }

    if (log_config.file && log_config.fenabled)
    {
        if (log_config.flevel < 0 && ((int)log * -1) < log_config.flevel)
            return;
        else if (log_config.flevel > 0 && (int)log < log_config.flevel)
            return;

        int i = (int)log - 1;

        time_t t = time(NULL);
        struct tm *lt = localtime(&t);

        char time[32];
        time[strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", lt)] = '\0';

        fprintf(log_config.file, "%s %5s %s at %s:%d: ", time, names[i], filename, funcname, line);

        va_list file_args;
        va_start(file_args, fmt);
        vfprintf(log_config.file, fmt, file_args);
        va_end(file_args);

        fprintf(log_config.file, "\n");
        fflush(log_config.file);
    }
}

#endif /* CMC_LOG_H */
