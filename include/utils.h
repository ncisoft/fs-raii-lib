#pragma once

#include <sys/time.h>
#ifdef __cplusplus
extern "C"
{
#endif
#include <config/config.h>
#include <logger.h>
#include <network.h>

#ifndef cast
#define cast(t, exp)  ((t)(exp))
#endif


typedef struct {
    const char *m_file;
    const char *m_func;
    int m_lineno;
} ut_logger_context_t;  /* ut: universal type */

extern void init_clock();
extern void msleep_select(long msec);
extern void msleep_usleep(long msec);
extern int  msleep(long msec);
extern void ut_print_time_elapsed_impl(ut_logger_context_t *ctx, char *keyword);
extern void _die(ut_logger_context_t* ut_ctx, int exit_code, const char *fmt, ...);

#define ut_logger_context_t_init(ctx) ut_logger_context_t ctx={.m_file = __FILE__, .m_func = __func__, .m_lineno = __LINE__}

#define die(exit_code, fmt, ...)                                               \
  do {                                                                         \
    ut_logger_context_t_init(ut_ctx);                                          \
    _die(&ut_ctx, exit_code, fmt, ##__VA_ARGS__);                              \
  } while (0)

#define ut_logger_trace(format, ...)  \
    ut_logger_context_t_init(ut_ctx); \
    logger_printf(LOGGER_LEVEL_TRACE, LOGGER_COLOR_CYAN, LOGGER_PREFIX format, "TRACE", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_debug(format, ...)  \
    ut_logger_context_t_init(ut_ctx); \
    logger_printf(LOGGER_LEVEL_DEBUG, LOGGER_COLOR_WHITE, LOGGER_PREFIX format, "DEBUG", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_info(format, ...)   \
    ut_logger_context_t_init(ut_ctx); \
    logger_printf(LOGGER_LEVEL_INFO, LOGGER_COLOR_GREEN, LOGGER_PREFIX format, "INFO", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_warn(format, ...)   \
    ut_logger_context_t_init(ut_ctx); \
    logger_printf(LOGGER_LEVEL_WARNING, LOGGER_COLOR_YELLOW, LOGGER_PREFIX format, "WARN", _ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_error(format, ...)  \
    ut_logger_context_t_init(ut_ctx); \
ut_logger_context_t_init;             \
    logger_printf(LOGGER_LEVEL_ERROR, LOGGER_COLOR_RED, LOGGER_PREFIX format, "ERROR", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

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
