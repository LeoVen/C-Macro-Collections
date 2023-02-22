# Configuration

## cmc_log_config

This is an anonymous static `struct` that can be directly modified to change logging levels, enable or disable output or change file output.

```c
static struct
{
    enum cmc_log_type tlevel;
    enum cmc_log_type flevel;
    bool              tenabled;
    bool              fenabled;
    bool              enabled;
    FILE *            file;
} cmc_log_config = { 0, 0, true, true, true, NULL };
```

__Members__

* `tlevel` - Terminal log level (`stderr`)
* `flevel` - File log level
* `tenabled` - Terminal logging enabled if `true`
* `fenabled` - File logging enabled if `true`
* `enabled` - Enables or disables all logs
* `FILE *file` - File for output

This configuration is global to every log. The `FILE` pointer is not handled by the API and needs to be able to write to the file.

## Log Level

By tuning `tlevel` and `flevel` (both accessed from `cmc_log_config`) you can filter which log messages are printed or not.

* If the log level is `0`, all logs are enabled
* If the log level `X` is positive:
    * All levels less than `X` will be disabled
    * If `X` is greater than the level of `FATAL`, all logs are disabled
* If the log level `X` is negative:
    * All levels greater than `abs(X)` are enabled
    * If `abs(X)` is greater than the level of `FATAL`, all logs are enabled

__Examples__

If you want to output only `TRACE` logs to a file and the rest to `stderr`:

* `cmc_log_config.tlevel = CMC_LOG_DEBUG`
* `cmc_log_config.flevel = -CMC_LOG_TRACE`

If you want to output only `FATAL` to `stderr` and the rest to a file:

* `cmc_log_config.tlevel = CMC_LOG_FATAL`
* `cmc_log_config.flevel = -CMC_LOG_ERROR`

If you want to output only `FATAL` and `ERROR` to a file an nothing to `stderr`:

* `cmc_log_config.tlevel = CMC_LOG_FATAL + 1`
* `cmc_log_config.flevel = CMC_LOG_ERROR`

__Summary__

* Log Level > 0: "Allow all log levels that are greater than this, including it"
* Log Level < 0: "Allow all log levels that are smaller than this, including it"
