#pragma once

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <ipc-bus/config.h>
#include <logger.h>
#include <ipc-bus/types.h>
#include <ipc-bus/buffer.h>
#include <ipc-bus/network.h>
#include <ipc-bus/fork.h>
#include <ipc-bus/raw-raii.h>
#include <ipc-bus/memory-pool.h>

#ifndef cast
#define cast(t, exp)  ((t)(exp))
#endif

typedef struct {
    const char *m_file;
    const char *m_func;
    int m_lineno;
} ut_logger_context_t;  /* ut: universal type */

enum {
  FD_STDIN = 0,  // STDIN_FILENO,
  FD_STDOUT = 1, // STDOUT_FILENO,
  FD_STDERR = 2, // STDERR_FILENO,

  __PADDING = 3

};

extern void init_clock();
extern void msleep_select(long msec);
extern void msleep_usleep(long msec);
extern int  msleep(long msec);
extern void ut_print_time_elapsed_impl(ut_logger_context_t *ctx, char *keyword);
extern void _die(ut_logger_context_t* ut_ctx, int exit_code, const char *fmt, ...);
extern uint32_t ut_pid_t_convert_uint32(pid_t pid);
extern void _ut_shorten_filename(ut_logger_context_t* ut_ctx);
#define comment(...) nop()
//#define nop(...) ((void) 0)
#define nop(msg, ...) ut_unused(##__VA_ARGS__)
extern void ut_unused();
#define label(...) nop()

#define ut_logger_context_t_init(ctx) ut_logger_context_t ctx={.m_file = __FILE__, .m_func = __func__, .m_lineno = __LINE__}

#define die(exit_code, fmt, ...)                                               \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _die(&ut_ctx, exit_code, fmt, ##__VA_ARGS__);                              \
  } while (0)

#define ut_logger_trace(format, ...)  \
    ut_logger_context_t_init(ut_ctx); \
    logger_printf(LOGGER_LEVEL_TRACE, LOGGER_COLOR_CYAN, LOGGER_PREFIX format, "TRACE", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define xt_logger_debug(format, ...)                                           \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _ut_shorten_filename(&ut_ctx);                                             \
    logger_printf(LOGGER_LEVEL_DEBUG, LOGGER_COLOR_WHITE,                      \
                  LOGGER_PREFIX format, "DEBUG", ut_ctx.m_file, ut_ctx.m_func, \
                  ut_ctx.m_lineno, ##__VA_ARGS__);                             \
  } while (0)

#define ut_logger_debug(format, ...)                                           \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _ut_shorten_filename(&ut_ctx);                                             \
    logger_printf(LOGGER_LEVEL_DEBUG, LOGGER_COLOR_WHITE,                      \
                  LOGGER_PREFIX format, "DEBUG", ut_ctx.m_file, ut_ctx.m_func, \
                  ut_ctx.m_lineno, ##__VA_ARGS__);                             \
  } while (0)

#define ut_logger_info(format, ...)                                            \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _ut_shorten_filename(&ut_ctx);                                             \
    logger_printf(LOGGER_LEVEL_INFO, LOGGER_COLOR_GREEN, LOGGER_PREFIX format, \
                  "INFO", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno,       \
                  ##__VA_ARGS__);                                              \
  } while (0)

#define ut_logger_warn(format, ...)                                            \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _ut_shorten_filename(&ut_ctx);                                             \
    logger_printf(LOGGER_LEVEL_WARNING, LOGGER_COLOR_YELLOW,                   \
                  LOGGER_PREFIX format, "WARN", ut_ctx.m_file, ut_ctx.m_func,  \
                  ut_ctx.m_lineno, ##__VA_ARGS__);                             \
  } while (0)

#define ut_logger_error(format, ...)                                           \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _ut_shorten_filename(&ut_ctx);                                             \
    logger_printf(LOGGER_LEVEL_ERROR, LOGGER_COLOR_RED, LOGGER_PREFIX format,  \
                  "ERROR", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno,      \
                  ##__VA_ARGS__);                                              \
  } while (0)

#define ut_logger_fatal(format, ...)  \
    ut_logger_context_t_init(ut_ctx); \
    logger_printf(LOGGER_LEVEL_FATAL, LOGGER_COLOR_RED_BOLD, LOGGER_PREFIX format, "FATAL", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_print_time_elapsed(keyword)                                         \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    ut_print_time_elapsed_impl(&ut_ctx, keyword);                              \
  } while (0)

#ifdef __cplusplus
}
#endif
