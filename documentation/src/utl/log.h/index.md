# log.h

Simple and customizable logging macros. Messages can have a custom logging level and a custom file output. This customization can be done via the global variable [`cmc_log_config`](configuration.html#cmc_log_config). Logs are outputted to `stderr`.

## cmc_log_type

Defines the possible Log Levels. These are used to categorize log messages. The higher the number, the more severe or important that message is. Check out [here](meanings.html)

```c
enum cmc_log_type
{
    CMC_LOG_TRACE = 1,
    CMC_LOG_DEBUG = 2,
    CMC_LOG_INFO  = 3,
    CMC_LOG_WARN  = 4,
    CMC_LOG_ERROR = 5,
    CMC_LOG_FATAL = 6
};
```

## CMC_LOG_COLOR

If this macro is defined, logging will be outputted with [ANSI color escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code#Colors). If you are logging into a file, these escape codes are not printed.
